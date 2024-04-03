#ifndef TS_HPP
#define TS_HPP

#include "numbering.hpp"
#include "dynbitset.hpp"
#include <list>
#include <vector>

class TS {
public:
  struct single_transition {
    size_t action;
    size_t to;
    single_transition(size_t action_, size_t to_) : action(action_), to(to_) {}
  };
  typedef std::list<single_transition> transition;
  struct state {
    DynBitset ap;
    transition ts;
    state() : ap(0, false), ts() {}
  };

private:
  std::vector<state> states;
  std::vector<size_t> initial;
public:
  TS(size_t n_state) : states(n_state) {}
  TS &setInitial(size_t initial);
  TS &addState(size_t state, DynBitset &&ap);
  TS &addTransition(size_t from, size_t action, size_t to);
  void show(FILE *fp, const Numbering &ap, const Numbering &act) const;
};

TS readTS(FILE *fp, Numbering &ap, Numbering &act);

#endif
