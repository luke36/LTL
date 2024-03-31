#include "parse.hpp"
#include "util.hpp"

#include <stdexcept>
#include <map>

using std::runtime_error;

static inline int getc_space(FILE *fp) {
  int c;
  do {
    c = getc(fp);
  } while (isspace(c));
  return c;
}

static inline bool isvalidprop(int c) {
  return islower(c) || isdigit(c) || c == '_' || c == '-';
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

Formula parse(FILE *fp, const Numbering &map) {
  return parseUntil(fp, map);
}
