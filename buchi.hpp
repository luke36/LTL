#ifndef BUCHI_HPP
#define BUCHI_HPP

#include "gba.hpp"

class BA {
public:
  struct state;
  struct single_transition {
    DynBitset pos;
    DynBitset neg;
    state *to;
    single_transition(const DynBitset &pos_, const DynBitset &neg_, state *to_)
      : pos(pos_), neg(neg_), to(to_) {}
    single_transition(single_transition &&other) = default;

    bool operator==(const single_transition &other) const {
      return pos == other.pos && neg == other.neg && to == other.to;
    }

    bool subsume(const single_transition &other) const {
      return pos.subset(other.pos) && neg == other.neg && to == other.to;
    }
  };
  typedef std::list<single_transition> transition;

  struct state {
    size_t id;
    bool is_final;
    transition ts;
    state() : is_final(false), ts() {}
    state(state &&other) = default;
    bool operator==(const state &other) const;
  };

private:
  std::vector<state *> states;
  std::vector<state *> initial;
public:
  BA(const GBA &gba);
  ~BA() {
    for (auto s : states) {
      delete s;
    }
  }

  size_t nStates() const {
    return states.size();
  }

  const std::vector<state *> &initialStates() const {
    return initial;
  }

  void show(FILE *fp, const Numbering &map) const;
};

#endif
