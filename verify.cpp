#include "verify.hpp"
#include <stack>

static bool
satisfyAP(const DynBitset &ap, const DynBitset &pos, const DynBitset &neg) {
  return pos.subset(ap) && ap.exclude(neg);
}

struct prod_state {
  const TS::state *ts_state;
  const BA::state *ba_state;
  TS::transition::const_iterator ts_t;
  BA::transition::const_iterator ba_t;
  prod_state(const TS::state *ts_state_, const BA::state *ba_state_)
    : ts_state(ts_state_), ba_state(ba_state_),
      ts_t(ba_state_->ts.empty() ? ts_state_->ts.cend() : ts_state_->ts.cbegin()),
      ba_t(ba_state_->ts.cbegin()) {}
  size_t index(const TS &ts) const {
    return ba_state->id * ts.nStates() + ts_state->id;
  };
  bool isFinal() const {
    return ba_state->is_final;
  }
  bool isDone() const {
    return ts_t == ts_state->ts.cend();
  }
  void step() {
    ++ba_t;
    if (ba_t == ba_state->ts.cend()) {
      ba_t = ba_state->ts.cbegin();
      ++ts_t;
    }
  }
  std::optional<prod_state> genNext() const {
    if (satisfyAP(ts_t->to->ap, ba_t->pos, ba_t->neg)) {
      return prod_state(ts_t->to, ba_t->to);
    } else {
      return {};
    }
  }
  bool operator==(const prod_state &other) const {
    return
      ts_state == other.ts_state && ba_state == other.ba_state;
  }
};

static std::vector<const TS::state *> stackToVec(std::stack<prod_state> &s) {
  std::stack<const TS::state *> rev;
  while (!s.empty()) {
    rev.push(s.top().ts_state);
    s.pop();
  }
  std::vector<const TS::state *> ret;
  while (!rev.empty()) {
    ret.push_back(rev.top());
    rev.pop();
  }
  return ret;
}

static std::optional<std::vector<const TS::state *>>
cycleCheck(std::stack<prod_state> &frag, prod_state begin, DynBitset &visit, const TS &ts) {
  frag.push(begin);
  visit.add(begin.index(ts));

  do {
    auto &s = frag.top();
    if (s.isDone()) {
      frag.pop();
    } else {
      auto &&maybe_next = s.genNext();
      if (maybe_next) {
        prod_state &next = maybe_next.value();
        if (next == begin) {
          return stackToVec(frag);
        }
        size_t i = next.index(ts);
        if (!visit.has(i)) {
          frag.push(next);
          visit.add(i);
        }
      }
      s.step();
    }
  } while (!frag.empty());
  return {};
}

static std::optional<std::vector<const TS::state *>>
reachableCycle(std::stack<prod_state> &frag, prod_state begin,
               DynBitset &visit_outer, DynBitset &visit_inner,
               const TS &ts) {
  frag.push(begin);
  visit_outer.add(begin.index(ts));

  do {
    auto &s = frag.top();
    if (s.isDone()) {
      prod_state s_(s.ts_state, s.ba_state);
      frag.pop();
      if (s_.isFinal()) {
        std::stack<prod_state> loop;
        auto &&ret = cycleCheck(loop, s_, visit_inner, ts);
        if (ret) {
          return ret;
        }
      }
    } else {
      auto &&maybe_next = s.genNext();
      if (maybe_next) {
        prod_state &next = maybe_next.value();
        size_t i = next.index(ts);
        if (!visit_outer.has(i)) {
          frag.push(next);
          visit_outer.add(i);
        }
      }
      s.step();
    }
  } while (!frag.empty());

  return {};
}

verify_return productIsEmptyFrom(const TS &ts, const BA &ba, const TS::state *init) {
  std::stack<prod_state> stack;
  size_t n_prod_states = ts.nStates() * ba.nStates();
  DynBitset outer(n_prod_states, false);
  DynBitset inner(n_prod_states, false);
  for (auto &s2 : ba.initialStates()) {
    for (auto &t : s2->ts) {
      if (satisfyAP(init->ap, t.pos, t.neg)) {
        auto &&maybe_loop = reachableCycle(stack, {init, t.to}, outer, inner, ts);
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
        if (satisfyAP(s1->ap, t.pos, t.neg)) {
          prod_state begin(s1, t.to);
          if (!outer.has(begin.index(ts))) {
            auto &&maybe_loop = reachableCycle(stack, begin, outer, inner, ts);
            if (maybe_loop) {
              return {stackToVec(stack), std::move(maybe_loop.value())};
            }
          }
        }
      }
    }
  }
  return {};
}

void verify_return::show(FILE *fp, const Numbering &map) const {
  if (empty) {
    fprintf(fp, "\e[0;32mSatisfied.\e[0m");
  } else {
    fprintf(fp, "\e[1;31mUnsatisfied.\e[0m Counterexample:\n\t");
    for (auto s : finite_fragment) {
      fprintf(fp, "%lu(", s->id);
      auto &ap = s->ap;
      bool first = true;
      for (size_t i = 0; i < ap.size(); i++) {
        if (ap.has(i)) {
          if (!first) {
            fprintf(fp, ",");
          }
          fprintf(fp, "%s", map.toString(i)->c_str());
          first = false;
        }
      }
      fprintf(fp, ")");
      fprintf(fp, " --> ");
    }

    fprintf(fp, "[ ");

    for (auto s : loop_fragment) {
      fprintf(fp, "%lu(", s->id);
      auto &ap = s->ap;
      bool first = true;
      for (size_t i = 0; i < ap.size(); i++) {
        if (ap.has(i)) {
          if (!first) {
            fprintf(fp, ",");
          }
          fprintf(fp, "%s", map.toString(i)->c_str());
          first = false;
        }
      }
      fprintf(fp, ")");
      fprintf(fp, " --> ");
    }

    fprintf(fp, "... ]");
  }
}
