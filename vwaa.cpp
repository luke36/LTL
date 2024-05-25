#include "vwaa.hpp"
#include <optional>

std::optional<VWAA::single_transition>
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

VWAA::transition transitionCompose(const VWAA::transition &t1,
                                   const VWAA::transition &t2) {
  VWAA::transition all;

  for (auto &t : t1) {
    for (auto &s : t2) {
      std::optional<VWAA::single_transition> &&u = composeSingle(s, t);
      if (u) {
        addTransition(all, std::move(u.value()));
      }
    }
  }

  return all;
}

VWAA::transition &transitionUnion(VWAA::transition &t1,
                                  VWAA::transition &&t2) {
  for (auto &t : t2) {
    addTransition(t1, std::move(t));
  }
  return t1;
}

size_t VWAA::addState(transition &&t, bool is_final) {
  bool redundant = false;
  size_t equal_state;
  for (size_t fs = 0; fs < transitions.size(); fs++) {
    if (transitions[fs].size() == t.size() &&
        is_final == (final_states.find(fs) != final_states.end())) {
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
      final_states.insert(s);
    }
    return s;
  }
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
      final_states.erase(i);
      removed_states.add(i);
    }
  }
  return *this;
}

void showAP(FILE *fp, const Numbering &map,
            const DynBitset &pos, const DynBitset &neg) {
  bool nothing = true;
  for (size_t p = 0; p < pos.size(); p++) {
    if (pos.has(p)) {
      if (nothing) {
        nothing = false;
      } else {
        fprintf(fp, " /\\ ");
      }
      fprintf(fp, "%s", map.toString(p)->c_str());
    }
  }
  for (size_t n = 0; n < pos.size(); n++) {
    if (neg.has(n)) {
      if (nothing) {
        nothing = false;
      } else {
        fprintf(fp, " /\\ ");
      }
      fprintf(fp, "~%s", map.toString(n)->c_str());
    }
  }
  if (nothing) {
    fprintf(fp, "tt");
  }
}

void VWAA::show(FILE *fp, const Numbering &map) const {
  for (size_t i = 0; i < transitions.size(); i++) {
    if (!removed_states.has((i))) {
      fprintf(fp, "%lu:\t", i);
      for (auto &t : transitions[i]) {
        showAP(fp, map, t.pos, t.neg);
        fprintf(fp, "  ->  tt");
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
    if (final_states.find(i) != final_states.end()) {
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
