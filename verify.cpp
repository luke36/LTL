#include "verify.hpp"
#include <stack>

struct prod_state {
  const TS *ts;
  size_t ts_state;
  const BA::state *ba_state;
  prod_state(const TS *ts_, size_t ts_state_, const BA::state *ba_state_)
    : ts(ts_), ts_state(ts_state_), ba_state(ba_state_) {}
  size_t index() const {
    return ts_state * ts->nStates() + ba_state->id;
  };
  const TS::transition &TSTransition() const {
    return ts->getState(ts_state).ts;
  }
  const DynBitset &TSAP() const {
    return ts->getState(ts_state).ap;
  }
  const BA::transition &BATransition() const {
    return ba_state->ts;
  }
  bool isFinal() const {
    return ba_state->is_final;
  }
  bool operator==(const prod_state &other) const {
    return
      ts == other.ts && ts_state == other.ts_state && ba_state == other.ba_state;
  }
};

static bool
satisfyAP(const DynBitset &ap, const DynBitset &pos, const DynBitset &neg) {
  return pos.subset(ap) && ap.exclude(neg);
}

static bool
cycleCheck(std::stack<prod_state> &frag, prod_state s, DynBitset &visit) {
  frag.push(s);
  visit.add(s.index());

  do {
    bool done = true;
    auto &s_ = frag.top();
    for (auto &t1 : s_.TSTransition()) {
      for (auto &t2 : s_.BATransition()) {
        prod_state next(s_.ts, t1.to, t2.to);
        if (satisfyAP(next.TSAP(), t2.pos, t2.neg)) {
          if (next == s) {
            return true;
          } else if (!visit.has(next.index())) {
            frag.push(next);
            visit.add(next.index());
            done = false;
            break;
          }
        }
      }
      if (!done) {
        break;
      }
    }
    if (done) {
      frag.pop();
    }
  } while (!frag.empty());
  return false;
}

static bool
reachableCycle(std::stack<prod_state> &frag, prod_state s,
               DynBitset &visit_outer, DynBitset &visit_inner) {
  frag.push(s);
  visit_outer.add(s.index());

  do {
    auto &s_ = frag.top();
    bool expanded = true;

    for (auto &t1 : s_.TSTransition()) {
      for (auto &t2 : s_.BATransition()) {
        prod_state next(s_.ts, t1.to, t2.to);
        if (!visit_outer.has(next.index()) && satisfyAP(next.TSAP(), t2.pos, t2.neg)) {
          frag.push(next);
          visit_outer.add(next.index());
          expanded = false;
          break;
        }
      }
      if (!expanded) {
        break;
      }
    }

    if (expanded) {
      auto s = s_;
      frag.pop();
      if (s.isFinal()) {
        std::stack<prod_state> loop;
        if (cycleCheck(loop, s, visit_inner)) {
          return true;
        }
      }
    }
  } while (!frag.empty());

  return false;
}

bool productIsEmptyFrom(const TS &ts, const BA &ba, size_t init) {
  std::stack<prod_state> stack;
  size_t n_prod_states = ts.nStates() * ba.nStates();
  DynBitset outer(n_prod_states, false);
  DynBitset inner(n_prod_states, false);
  for (auto &s2 : ba.initialStates()) {
    for (auto &t : s2->ts) {
      if (satisfyAP(ts.getState(init).ap, t.pos, t.neg)) {
        if (reachableCycle(stack, {&ts, init, t.to}, outer, inner)) {
          return false;
        }
      }
    }
  }
  return true;
}

bool productIsEmpty(const TS &ts, const BA &ba) {
  std::stack<prod_state> stack;
  size_t n_prod_states = ts.nStates() * ba.nStates();
  DynBitset outer(n_prod_states, false);
  DynBitset inner(n_prod_states, false);
  for (auto &s1 : ts.getInitial()) {
    for (auto &s2 : ba.initialStates()) {
      for (auto &t : s2->ts) {
        if (satisfyAP(ts.getState(s1).ap, t.pos, t.neg)) {
          if (reachableCycle(stack, {&ts, s1, t.to}, outer, inner)) {
            return false;
          }
        }
      }
    }
  }
  return true;
}

