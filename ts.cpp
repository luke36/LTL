#include "ts.hpp"
#include "vwaa.hpp"

TS &TS::addState(size_t state, DynBitset &&ap) {
  states[state].ap = std::move(ap);
  states[state].id = state;
  return *this;
}

TS &TS::setInitial(size_t initial) {
  this->initial.push_back(states + initial);
  return *this;
}

TS &TS::addTransition(size_t from, size_t action, size_t to) {
  states[from].ts.emplace_back(single_transition(action, states + to));
  return *this;
}

size_t TS::nStates() const {
  return n_state;
}

const TS::state &TS::getState(size_t i) const {
  return states[i];
}

const std::vector<TS::state *> &TS::getInitial() const {
  return initial;
}

void TS::show(FILE *fp, const Numbering &ap, const Numbering &act) const {
  for (size_t i = 0; i < n_state; i++) {
    fprintf(fp, "%lu:", i);
    const DynBitset &s = states[i].ap;
    for (size_t i = 0; i < s.size(); i++) {
      if (s.has(i)) {
        fprintf(fp, " %s", ap.toString(i)->c_str());
      }
    }
    for (auto &t : states[i].ts) {
      fprintf(fp, "\n\t--%s-->  %lu", act.toString(t.action)->c_str(), t.to->id);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "initial states:");
  for (auto s : initial) {
    fprintf(fp, " %lu", s->id);
  }
  fprintf(fp, "\n");
}

static bool endOfLine(FILE *fp) {
  int c;
  do {
    c = getc(fp);
  } while (c == '\t' || c == ' ');
  if (c == '\n' || c == EOF) {
    return true;
  } else {
    ungetc(c, fp);
    return false;
  }
}

TS readTS(FILE *fp, Numbering &ap, Numbering &act) {
  size_t n_state, n_transition;
  fscanf(fp, "%lu %lu", &n_state, &n_transition);

  while (endOfLine(fp))
    ; // skip newlines

  TS ts(n_state);

  while (!endOfLine(fp)) {
    size_t n;
    fscanf(fp, "%lu", &n);
    ts.setInitial(n);
  }

  while (endOfLine(fp))
    ;

  while (!endOfLine(fp)) {
    std::string s;
    int c;
    while (!isspace(c = getc(fp))) {
      s += c;
    }
    act.addString(std::move(s));
    if (c == '\n' || c == EOF) {
      break;
    }
  }

  while (endOfLine(fp))
    ;

  size_t n_ap = 0;
  while (!endOfLine(fp)) {
    n_ap++;
    std::string s;
    int c;
    while (!isspace(c = getc(fp))) {
      s += c;
    }
    ap.addString(std::move(s));
    if (c == '\n' || c == EOF) {
      break;
    }
  }

  while (endOfLine(fp))
    ;

  for (size_t i = 0; i < n_transition; i++) {
    size_t from, act, to;
    fscanf(fp, "%lu %lu %lu", &from, &act, &to);
    ts.addTransition(from, act, to);
  }

  for (size_t i = 0; i < n_state; i++) {
    DynBitset ap(n_ap, false);
    char c;
    while (endOfLine(fp))
      ;
    do {
      if ((c = getc(fp)) == '-') {
        int _;
        fscanf(fp, "%d", &_);
        getc(fp);
        break;
      } else {
        ungetc(c, fp);
        size_t n;
        fscanf(fp, "%lu", &n);
        ap.add(n);
      }
    } while (!endOfLine(fp));
    ts.addState(i, std::move(ap));
  }

  return ts;
}
