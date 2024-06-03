#include "parse.hpp"
#include <stdexcept>

using std::runtime_error;

static int getc_space(FILE *fp) {
  int c;
  do {
    c = getc(fp);
  } while (isspace(c));
  return c;
}

static bool isvalidprop(int c) {
  return islower(c) || isdigit(c) || c == '_';
}

static Formula parseSingle(FILE *fp, const Numbering &map) {
  int c = getc_space(fp);

  switch (c) {
  case '(': {
    Formula f = parse(fp, map);
    c = getc_space(fp);
    if (c != ')') {
      throw runtime_error("parse: Unmatched parenthesis");
    } else {
      return f;
    }
  }
  case '!':
    return mkNegation(parseSingle(fp, map));
  case 'X':
    return mkNext(parseSingle(fp, map));
  case 'G':
    return mkAlways(parseSingle(fp, map));
  case 'F':
    return mkEvent(parseSingle(fp, map));
  default:
    if (!isvalidprop(c)) {
      throw runtime_error("parse: Expected proposition, but got ...");
    } else {
      std::string s;
      do {
        s += c;
        c = getc(fp);
      } while (isvalidprop(c));
      ungetc(c, fp);
      auto ret = map.toId(s);
      if (!ret) {
        throw runtime_error("parse: Unknown atom proposition");
      } else {
        return mkAtom(ret.value());
      }
    }
  }
}

static Formula parseConj(FILE *fp, const Numbering &map) {
  Formula first = parseSingle(fp, map);
  int c = getc_space(fp);
  switch (c) {
  case '/':
    if (getc(fp) != '\\') {
      throw runtime_error("parse: Expected ‘\\’ after ‘/’");
    } else {
      return mkConj(first, parseConj(fp, map));
    }
  default:
    ungetc(c, fp);
    return first;
  }
}

static Formula parseDisj(FILE *fp, const Numbering &map) {
  Formula first = parseConj(fp, map);
  int c = getc_space(fp);
  switch (c) {
  case '\\':
    if (getc(fp) != '/') {
      throw runtime_error("parse: Expected ‘/’ after ‘\\’");
    } else {
      return mkDisj(first, parseDisj(fp, map));
    }
  default:
    ungetc(c, fp);
    return first;
  }
}

static Formula parseUntil(FILE *fp, const Numbering &map) {
  Formula first = parseDisj(fp, map);
  int c = getc_space(fp);
  switch (c) {
  case 'U':
    return mkUntil(first, parseUntil(fp, map));
  default:
    ungetc(c, fp);
    return first;
  }
}

static Formula parseImply(FILE *fp, const Numbering &map) {
  Formula first = parseUntil(fp, map);
  int c = getc_space(fp);
  switch (c) {
  case '-':
    if (getc(fp) != '>') {
      throw runtime_error("parse: Expected ‘>’ after ‘-’");
    } else {
      return mkImply(first, parseImply(fp, map));
    }
  default:
    ungetc(c, fp);
    return first;
  }
}

Formula parse(FILE *fp, const Numbering &map) {
  return parseImply(fp, map);
}

static void skipLowerEqual(char *&str) {
  while (islower(*(++str)))
    ;
  if (*str != '=') {
    throw runtime_error("parse: Unknown command-line argument");
  } else {
    str++;
  }
}

void parseCLIArgs(int argc, char *argv[],
                  FILE *&ts, FILE *&formula, FILE *&output) {
  for (int i = 1; i < argc; i++) {
    char *str = argv[i];
    while (*(++str) == '-')
      ;
    switch (*str) {
    case 't':
      skipLowerEqual(str);
      ts = fopen(str, "r");
      break;
    case 'f':
      skipLowerEqual(str);
      formula = fopen(str, "r");
      break;
    case 'o':
      skipLowerEqual(str);
      output = fopen(str, "w");
      break;
    default:
      throw runtime_error("parse: Unknown command-line argument");
    }
  }
}

