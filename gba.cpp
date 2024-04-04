#include "gba.hpp"

bool GBA::single_transition::operator==(const single_transition &other) const {
  return final_set == other.final_set && t == other.t;
}

bool GBA::single_transition::subsume(const single_transition &other) const {
  return
    other.final_set.subset(final_set) && t.subsume(other.t);
}

bool GBA::hasState(const DynBitset &state) const {
  return
    transitions.find(state) != transitions.end() ||
    redirect.find(state) != redirect.end();
}

const std::map<DynBitset, GBA::transition> &GBA::allStates() const {
  return transitions;
}

const std::vector<DynBitset> &GBA::initialStates() const {
  return initial;
}

size_t GBA::nFinal() const {
  return n_final;
}

static GBA::transition &addTransition(GBA::transition &t1,
                                      GBA::single_transition &&t) {
  bool redundant = false;
  for (auto i = t1.cbegin(); i != t1.cend();) {
    if (i->subsume(t)) {
      redundant = true;
      break;
    } else if (t.subsume(*i)) {
      i = t1.erase(i);
    } else {
      ++i;
    }
  }
  if (!redundant) {
    t1.emplace_back(std::move(t));
  }
  return t1;
}

static DynBitset computeFinal(const VWAA::single_transition &t, const VWAA &aa) {
  const std::set<size_t> final_states = aa.finalStates();
  DynBitset ret(aa.finalStates().size(), false);
  size_t i_f = final_states.size() - 1; // this order seems to generate less states
  for (auto &s : aa.finalStates()) {
    if (!t.to.has(s)) {
      ret.add(i_f);
    } else {
      for (auto &tt : aa.stateTransition(s)) {
        if (tt.subsume(t) && !tt.to.has(s)) {
          ret.add(i_f);
          break;
        }
      }
    }
    i_f--;
  }
  return ret;
}

static void composeRec(const std::vector<const VWAA::transition *> &ts,
                       size_t i,
                       std::vector<VWAA::single_transition> &prod,
                       const VWAA &aa,
                       GBA::transition &all) {
  if (i == prod.size()) {
    VWAA::single_transition &t = prod.back();
    DynBitset &&final_set = computeFinal(t, aa);
    addTransition(all, GBA::single_transition(std::move(final_set), std::move(t)));
  } else {
    for (auto &t : *ts[i]) {
      if (i == 0) {
        prod[i] = t;
        composeRec(ts, i + 1, prod, aa, all);
      } else {
        std::optional<VWAA::single_transition> &&s = composeSingle(prod[i - 1], t);
        if (s) {
          prod[i] = std::move(s.value());
          composeRec(ts, i + 1, prod, aa, all);
        }
      }
    }
  }
}

static GBA::transition
transitionCompose(const std::vector<const VWAA::transition *> ts, const VWAA &aa) {
  std::vector<VWAA::single_transition> prefix_prod;
  GBA::transition all;

  if (ts.empty()) {
    VWAA::single_transition triv(aa.nAP(), aa.MaxState());
    auto &&all_final = computeFinal(triv, aa);
    all.emplace_back(std::move(all_final), std::move(triv));
  } else {
    // we do not have an empty constructor
    for (auto &t : ts) {
      if (t->empty()) {
        return all;
      } else {
        prefix_prod.push_back(t->front());
      }
    }
    composeRec(ts, 0, prefix_prod, aa, all);
  }

  return all;
}

GBA &GBA::addState(DynBitset &&state, transition &&t) {
  for (auto &tt : t) {
    auto r = redirect.find(tt.t.to);
    if (r != redirect.end()) {
      tt.t.to = r->second;
    }
  }

  bool redundant = false;
  const DynBitset *equal_state = nullptr;
  for (auto &s : transitions) {
    if (s.second.size() == t.size()) {
      bool equal = true;
      for (auto &t1 : s.second) {
        bool same = false;
        for (auto &t2 : t) {
          if (t1 == t2) {
            same = true;
            break;
          }
        }
        if (!same) {
          equal = false;
          break;
        }
      }
      if (equal) {
        redundant = true;
        equal_state = &s.first;
      }
    }
  }

  if (redundant) {
    for (auto &s : transitions) {
      for (auto &tt : s.second) {
        if (tt.t.to == state) {
          tt.t.to = *equal_state;
        }
      }
    }
    bool is_initial = false;
    for (auto &s : initial) {
      if (s == *equal_state) {
        is_initial = true;
        break;
      }
    }
    for (auto i = initial.begin(); i != initial.end(); ++i) {
      if (*i == state) {
        if (is_initial) {
          initial.erase(i);
        } else {
          *i = *equal_state;
        }
        break;
      }
    }
    redirect.insert({std::move(state), *equal_state});
  } else {
    transitions.insert({std::move(state), std::move(t)});
  }
  return *this;
}

GBA &GBA::finalize() {
  redirect.clear();
  return *this;
}

#include <stack>

GBA genGBA(const VWAA &aa) {
  GBA ba(aa.initialStates(), aa.finalStates().size());

  std::stack<DynBitset> work;
  for (auto &s : aa.initialStates()) {
    work.push(s);
  }
  while (!work.empty()) {
    DynBitset s = work.top();
    work.pop();
    if (!ba.hasState(s)) {
      std::vector<const VWAA::transition *> ts;
      for (size_t i = 0; i < aa.MaxState(); i++) {
        if (s.has(i)) {
          ts.push_back(&aa.stateTransition(i));
        }
      }
      auto t = transitionCompose(ts, aa);
      for (auto &tt : t) {
        work.push(tt.t.to);
      }
      ba.addState(std::move(s), std::move(t));
    }
  }
  return ba.finalize();
}

void GBA::show(FILE *fp, const Numbering &map) const {
  std::map<DynBitset, size_t> number;
  size_t n = 0;
  for (auto &s : transitions) {
    number.insert({s.first, n++});
  }
  for (auto i = transitions.cbegin(); i != transitions.cend(); ++i) {
    size_t n = number[i->first];
    fprintf(fp, "%lu:\t", n);
    for (auto &t : i->second) {
      showAP(fp, map, t.t.pos, t.t.neg);
      fprintf(fp, "  ->  %lu", number[t.t.to]);
      fprintf(fp, "  |");
      for (size_t j = 0; j < t.final_set.size(); j++) {
        if (t.final_set.has(j)) {
          fprintf(fp, " %lu", j);
        }
      }
      fprintf(fp, "\n\t");
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "initial states:");
  for (auto &i : initial) {
    fprintf(fp, " %lu", number[i]);
  }
  fprintf(fp, "\n");
}
