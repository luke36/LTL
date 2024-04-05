#ifndef TS_HPP
#define TS_HPP

#include "numbering.hpp"
#include "dynbitset.hpp"
#include <list>
#include <vector>

class TS {
public:
  struct state;
  struct single_transition {
    size_t action;
    state *to;
    single_transition(size_t action_, state *to_) : action(action_), to(to_) {}
  };
  typedef std::list<single_transition> transition;
  struct state {
    size_t id;
    DynBitset ap;
    transition ts;
    state() : ap(0, false), ts() {}
  };

private:
  size_t n_state;
  state *states;
  std::vector<state *> initial;
public:
  TS(size_t n_state_) : n_state(n_state_), states(new state [n_state_]) {}
  ~TS() { delete [] states; }
  TS &setInitial(size_t initial);
  TS &addState(size_t state, DynBitset &&ap);
  TS &addTransition(size_t from, size_t action, size_t to);
  size_t nStates() const;
  const std::vector<state *> &getInitial() const;
  const state &getState(size_t i) const;
  void show(FILE *fp, const Numbering &ap, const Numbering &act) const;
};

TS readTS(FILE *fp, Numbering &ap, Numbering &act);

#endif
