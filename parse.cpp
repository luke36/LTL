#include "parse.hpp"
#include "util.hpp"

#include <map>

static inline int getc_space(FILE *fp) {
  int c;
  do {
    c = getc(fp);
  } while (isspace(c));
  return c;
}

static inline bool isvalidprop(int c) {
  return isalnum(c) || c == '_' || c == '-';
}

static Formula parseSingle(FILE *fp, std::map<std::string, size_t> to_id) {
  int c = getc_space(fp);

  switch (c) {
  case '(': {
    Formula f = parse(fp, to_id);
    c = getc_space(fp);
    if (c != ')') {
      fail("parse: Unmatched parenthesis");
    } else {
      return f;
    }
  }
  case '!':
    return mkNegation(parseSingle(fp, to_id));
  case 'X':
    return mkNext(parseSingle(fp, to_id));
  case 'G':
    return mkAlways(parseSingle(fp, to_id));
  case 'F':
    return mkEvent(parseSingle(fp, to_id));
  default:
    if (!isvalidprop(c)) {
      fail("parse: Expected proposition, but got %c", c);
    } else {
      std::string s;
      do {
        s += c;
        c = getc(fp);
      } while (isalnum(c) || c == '_' || c == '-');
      ungetc(c, fp);
      auto ret = to_id.find(s);
      if (ret == to_id.end()) {
        fail("parse: Unknown atom proposition ‘%s’", s.c_str());
      } else {
        return mkAtom(ret->second, s);
      }
    }
  }
}

static Formula parseConj(FILE *fp, std::map<std::string, size_t> to_id) {
  Formula first = parseSingle(fp, to_id);
  int c = getc_space(fp);
  switch (c) {
  case '/':
    if (getc(fp) != '\\') {
      fail("parse: Expected ‘\\’ after ‘/’");
    } else {
      return mkConj(first, parseConj(fp, to_id));
    }
  default:
    ungetc(c, fp);
    return first;
  }
}

static Formula parseDisj(FILE *fp, std::map<std::string, size_t> to_id) {
  Formula first = parseConj(fp, to_id);
  int c = getc_space(fp);
  switch (c) {
  case '\\':
    if (getc(fp) != '/') {
      fail("parse: Expected ‘/’ after ‘\\’");
    } else {
      return mkDisj(first, parseDisj(fp, to_id));
    }
  default:
    ungetc(c, fp);
    return first;
  }
}

static Formula parseUntil(FILE *fp, std::map<std::string, size_t> to_id) {
  Formula first = parseDisj(fp, to_id);
  int c = getc_space(fp);
  switch (c) {
  case 'U':
    return mkUntil(first, parseUntil(fp, to_id));
  default:
    ungetc(c, fp);
    return first;
  }
}

Formula parse(FILE *fp, std::map<std::string, size_t> to_id) {
  return parseUntil(fp, to_id);
}
