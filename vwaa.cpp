#include "vwaa.hpp"
#include <optional>

bool VWAA::single_transition::subsume(const single_transition &other) const {
  return
    pos.subset(other.pos) &&
    neg.subset(other.neg) &&
    to.subset(other.to);
}

bool VWAA::single_transition::operator==(const single_transition &other) const {
  return
    pos == other.pos &&
    neg == other.neg &&
    to == other.to;
}

static std::optional<VWAA::single_transition>
composeSingle(const VWAA::single_transition &t1, const VWAA::single_transition &t2) {
  VWAA::single_transition ret(t1);
  ret.pos.unify(t2.pos);
  ret.neg.unify(t2.neg);
  ret.to.unify(t2.to);
  if (!ret.pos.exclude(ret.neg)) {
    return {};
  } else {
    return ret;
  }
}

static VWAA::transition &addTransition(VWAA::transition &t1,
                                       VWAA::single_transition &&t) {
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

static void composeRec(VWAA::transition &all,
                       std::vector<VWAA::single_transition> &prod,
                       const std::vector<const VWAA::transition *> &ts,
                       size_t i) {
  if (i == prod.size()) {
    addTransition(all, std::move(prod.back()));
  } else {
    for (auto &t : *ts[i]) {
      if (i == 0) {
        prod[i] = t;
        composeRec(all, prod, ts, i + 1);
      } else {
        std::optional<VWAA::single_transition> &&s = composeSingle(prod[i - 1], t);
        if (s) {
          prod[i] = std::move(s.value());
          composeRec(all, prod, ts, i + 1);
        }
      }
    }
  }
}

VWAA::transition transitionCompose(const std::vector<const VWAA::transition *> &ts) {
  std::vector<VWAA::single_transition> prefix_prod(ts.size());
  VWAA::transition all;

  composeRec(all, prefix_prod, ts, 0);

  return all;
}

VWAA::transition &transitionUnion(VWAA::transition &t1,
                                  VWAA::transition &&t2) {
  for (auto &t : t2) {
    addTransition(t1, std::move(t));
  }
  return t1;
}

size_t VWAA::nextStateId() const {
  return transitions.size();
}

size_t VWAA::addState(transition &&t, bool is_final) {
  bool redundant = false;
  size_t equal_state;
  for (size_t fs = 0; fs < transitions.size(); fs++) {
    if (transitions[fs].size() == t.size() &&
        is_final == final_states.has(fs)) {
      bool equal = true;
      for (auto &t1 : transitions[fs]) {
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
        equal_state = fs;
      }
    }
  }

  if (redundant) {
    return equal_state;
  } else {
    size_t s = transitions.size();
    transitions.emplace_back(std::move(t));
    if (is_final) {
      final_states.add(s);
    }
    return s;
  }
}

VWAA::transition &VWAA::stateTransition(size_t s) {
  return transitions[s];
}

const std::vector<DynBitset> &VWAA::initialStates() const {
  return initial_states;
}

const DynBitset &VWAA::finalStates() const {
  return final_states;
}

VWAA &VWAA::setInitialStates(std::vector<DynBitset> &&initial) {
  initial_states = std::move(initial);
  DynBitset reachable = DynBitset(transitions.size(), false);
  for (auto &s : initial_states) {
    reachable.unify(s);
  }
  for (long long i = transitions.size() - 1; i >= 0; i--) {
    if (reachable.has(i)) {
      for (auto &t : transitions[i]) {
        reachable.unify(t.to);
      }
    } else {
      transitions[i] = {};
      final_states.remove(i);
      removed_states.add(i);
    }
  }
  return *this;
}

void VWAA::show(FILE *fp, const Numbering &map) const {
  for (size_t i = 0; i < transitions.size(); i++) {
    if (!removed_states.has((i))) {
      fprintf(fp, "%lu:\t", i);
      for (auto &t : transitions[i]) {
        fprintf(fp, "tt");
        for (size_t j = 0; j < t.pos.size(); j++) {
          if (t.pos.has(j)) {
            fprintf(fp, " /\\ %s", map.toString(j)->c_str());
          }
        }
        for (size_t j = 0; j < t.neg.size(); j++) {
          if (t.neg.has(j)) {
            fprintf(fp, " /\\ ~%s", map.toString(j)->c_str());
          }
        }
        printf("  ->  tt");
        for (size_t j = 0; j < t.to.size(); j++) {
          if (t.to.has(j)) {
            fprintf(fp, " /\\ %lu", j);
          }
        }
        fprintf(fp, "\n\t");
      }
      fprintf(fp, "\n");
    }
  }

  fprintf(fp, "final states: ");
  for (size_t i = 0; i < transitions.size(); i++) {
    if (final_states.has(i)) {
      fprintf(fp, "%lu ", i);
    }
  }
  fprintf(fp, "\n");

  fprintf(fp, "initial states:\n");
  for (auto &s : initial_states) {
    fprintf(fp, "\ttt");
    for (size_t j = 0; j < s.size(); j++) {
        if (s.has(j)) {
          fprintf(fp, " /\\ %lu", j);
        }
      }
    fprintf(fp, "\n");
  }
}
