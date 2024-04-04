#include "verify.hpp"
#include <stack>

struct prod_state {
  const TS *ts;
  size_t ts_state;
  const BA::state *ba_state;
  prod_state(const TS *ts_, size_t ts_state_, const BA::state *ba_state_)
    : ts(ts_), ts_state(ts_state_), ba_state(ba_state_) {}
  size_t index() const {
    return ba_state->id * ts->nStates() + ts_state;
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

static std::vector<size_t> stackToVec(std::stack<prod_state> &s) {
  std::stack<size_t> rev;
  while (!s.empty()) {
    rev.push(s.top().ts_state);
    s.pop();
  }
  std::vector<size_t> ret;
  while (!rev.empty()) {
    ret.push_back(rev.top());
    rev.pop();
  }
  return ret;
}

static std::optional<std::vector<size_t>>
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
            return stackToVec(frag);
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
  return {};
}

static std::optional<std::vector<size_t>>
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
        auto &&ret = cycleCheck(loop, s, visit_inner);
        if (ret) {
          return ret;
        }
      }
    }
  } while (!frag.empty());

  return {};
}

verify_return productIsEmptyFrom(const TS &ts, const BA &ba, size_t init) {
  std::stack<prod_state> stack;
  size_t n_prod_states = ts.nStates() * ba.nStates();
  DynBitset outer(n_prod_states, false);
  DynBitset inner(n_prod_states, false);
  for (auto &s2 : ba.initialStates()) {
    for (auto &t : s2->ts) {
      if (satisfyAP(ts.getState(init).ap, t.pos, t.neg)) {
        auto &&maybe_loop = reachableCycle(stack, {&ts, init, t.to}, outer, inner);
        if (maybe_loop) {
          return {stackToVec(stack), std::move(maybe_loop.value())};
        }
      }
    }
  }
  return {};
}

verify_return productIsEmpty(const TS &ts, const BA &ba) {
  std::stack<prod_state> stack;
  size_t n_prod_states = ts.nStates() * ba.nStates();
  DynBitset outer(n_prod_states, false);
  DynBitset inner(n_prod_states, false);
  for (auto &s1 : ts.getInitial()) {
    for (auto &s2 : ba.initialStates()) {
      for (auto &t : s2->ts) {
        if (satisfyAP(ts.getState(s1).ap, t.pos, t.neg)) {
          auto &&maybe_loop = reachableCycle(stack, {&ts, s1, t.to}, outer, inner);
          if (maybe_loop) {
            return {stackToVec(stack), std::move(maybe_loop.value())};
          }
        }
      }
    }
  }
  return {};
}

void verify_return::show(FILE *fp, const TS &ts, const Numbering &map) const {
  if (empty) {
    fprintf(fp, "satisfied.");
  } else {
    fprintf(fp, "unsatisfied, counterexample:\n\t");
    for (auto s : finite_fragment) {
      fprintf(fp, "%lu {", s);
      auto &ap = ts.getState(s).ap;
      for (size_t i = 0; i < ap.size(); i++) {
        if (ap.has(i)) {
          fprintf(fp, " %s", map.toString(i)->c_str());
        }
      }
      fprintf(fp, " }");
      fprintf(fp, " --> ");
    }

    fprintf(fp, "( ");

    for (auto s : loop_fragment) {
      fprintf(fp, "%lu {", s);
      auto &ap = ts.getState(s).ap;
      for (size_t i = 0; i < ap.size(); i++) {
        if (ap.has(i)) {
          fprintf(fp, " %s", map.toString(i)->c_str());
        }
      }
      fprintf(fp, " }");
      fprintf(fp, " --> ");
    }

    fprintf(fp, "... )");
  }
}
