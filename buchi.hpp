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
    bool operator==(const single_transition &other) const;
    bool subsume(const single_transition &other) const;
  };
  typedef std::list<single_transition> transition;

  struct state {
    size_t id; // 0 means undone
    bool is_final;
    transition ts;
    state() : id(0), is_final(false), ts() {}
    state(state &&other) = default;
    bool operator==(const state &other) const;
    bool unfilled() const;
  };

private:
  std::vector<state *> states;
  std::vector<state *> initial;
public:
  BA(const GBA &gba);
  ~BA();
  void show(FILE *fp, const Numbering &map) const;
};

#endif
