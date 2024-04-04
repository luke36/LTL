#include "buchi.hpp"

bool BA::single_transition::operator==(const BA::single_transition &other) const {
  return pos == other.pos && neg == other.neg && to == other.to;
}

bool BA::single_transition::subsume(const BA::single_transition &other) const {
  return pos.subset(other.pos) && neg == other.neg && to == other.to;
}

bool BA::state::operator==(const state &other) const {
  if (is_final != other.is_final) {
    return false;
  }
  if (ts.size() != other.ts.size()) {
    return false;
  }
  for (auto &t : ts) {
    bool same = false;
    for (auto &s : other.ts) {
      if (s == t) {
        same = true;
        break;
      }
    }
    if (!same) {
      return false;
    }
  }
  return true;
}

static void addTransition(BA::transition &ts, BA::single_transition &&t) {
  bool redundant = false;
  for (auto i = ts.cbegin(); i != ts.cend();) {
    if (i->subsume(t)) {
      redundant = true;
      break;
    } else if (t.subsume(*i)) {
      i = ts.erase(i);
    } else {
      ++i;
    }
  }
  if (!redundant) {
    ts.emplace_back(std::move(t));
  }
}

static BA::state *addState(struct BA::state *s, std::vector<BA::state *> &states,
                           std::vector<BA::state *> &initial) {
  bool redundant = false;
  BA::state *equal_s = nullptr;
  for (auto s_ : states) {
    if (*s == *s_) {
      redundant = true;
      equal_s = s_;
      break;
    }
  }

  if (redundant) {
    for (auto s_ : states) {
      for (auto &t : s_->ts) {
        if (t.to == s) {
          t.to = equal_s;
        }
      }
    }
    bool in_initial = false;
    for (auto s : initial) {
      if (s == equal_s) {
        in_initial = true;
        break;
      }
    }
    for (auto i = initial.begin(); i != initial.end(); ++i) {
      if (*i == s) {
        if (in_initial) {
          initial.erase(i);
        } else {
          *i = equal_s;
        }
        break;
      }
    }
    delete s;
    return equal_s;
  } else {
    s->id = states.size() + 1; // > 0
    states.push_back(std::move(s));
    return s;
  }
}

static size_t allFinalFrom(size_t j, const DynBitset &js) {
  for (; j < js.size(); j++) {
    if (!js.has(j)) {
      break;
    }
  }
  return j;
}

static BA::state *unfilledState() {
  BA::state *s = new BA::state();
  s->id = 0;
  return s;
}

static bool isUnfilled(BA::state *s) {
  return s->id == 0;
}

static void finalizeID(std::vector<BA::state *> ss) {
  for (auto s : ss) {
    s->id--;
  }
}

#include <stack>

BA::BA(const GBA &gba) {
  auto &states = gba.allStates();
  size_t n_final = gba.nFinal();

  std::map<DynBitset, std::vector<state *>> to_state;
  std::vector<state *> done;
  std::stack<std::pair<const DynBitset *, size_t>> work;

  for (auto &s : states) {
    to_state[s.first] = std::vector<state *>(n_final + 1, nullptr);
  }
  for (auto &s : gba.initialStates()) {
    work.push({&s, 0});
    state *init = unfilledState();
    to_state.find(s)->second[0] = init;
    initial.push_back(init);
  }
  while (!work.empty()) {
    auto p = work.top();
    work.pop();
    auto x = p.first;
    size_t j = p.second;
    auto &v = to_state.find(*x)->second;
    auto s = v[j];
    if (isUnfilled(s)) { // undone
      transition ts;
      if (j == n_final) {
        s->is_final = true;
      }
      for (auto &t : states.find(*x)->second) {
        size_t k = allFinalFrom(j == n_final ? 0 : j, t.final_set);
        auto &w = to_state.find(t.t.to)->second;
        if (w[k] == nullptr) {
          w[k] = unfilledState();
          work.push({&t.t.to, k});
        }
        addTransition(ts, single_transition(t.t.pos, t.t.neg, w[k]));
      }
      s->ts = std::move(ts);
      state *added = addState(s, done, initial);
      v[j] = added;
    }
  }
  this->states = std::move(done);
  finalizeID(this->states);
}

BA::~BA() {
  for (auto s : states) {
    delete s;
  }
}

size_t BA::nStates() const {
  return states.size();
}

const std::vector<BA::state *> &BA::initialStates() const {
  return initial;
}

void BA::show(FILE *fp, const Numbering &map) const {
  for (auto s : states) {
    if (s->is_final) {
      fprintf(fp, "F ");
    } else {
      fprintf(fp, "N ");
    }
    fprintf(fp, "%lu:\t", s->id);
    for (auto &t : s->ts) {
      showAP(fp, map, t.pos, t.neg);
      fprintf(fp, "  ->  %lu\n\t", t.to->id);
    }
    fprintf(fp, "\n");
  }
  fprintf(fp, "initial states:");
  for (auto &i : initial) {
    fprintf(fp, " %lu", i->id);
  }
  fprintf(fp, "\n");
}
