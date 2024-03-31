#ifndef VWAA_HPP
#define VWAA_HPP

#include "dynbitset.hpp"
#include "numbering.hpp"
#include <set>
#include <vector>

class VWAA {
public:
  struct single_transition {
    DynBitset pos;
    DynBitset neg;
    DynBitset to;
    single_transition() {}
    single_transition(size_t ap, size_t max)
      : pos(ap, false), neg(ap, false), to(max, false) {};
    single_transition(const DynBitset &pos_,
                      const DynBitset &neg_,
                      const DynBitset &to_)
      : pos(pos_), neg(neg_), to(to_) {}
    single_transition(const DynBitset &&pos_,
                      const DynBitset &&neg_,
                      const DynBitset &&to_)
      : pos(std::move(pos_)), neg(std::move(neg_)), to(std::move(to_)) {}
    single_transition(const single_transition &other) = default;
    single_transition(single_transition &&other) = default;
    single_transition &operator=(const single_transition &other) = default;
    single_transition &operator=(single_transition &&other) = default;
    bool operator==(const single_transition &other) const;
    bool subsume(const single_transition &other) const;
  };
  typedef std::vector<single_transition> transition;

private:
  size_t n_ap;
  size_t max_state;
  std::vector<transition> transitions;
  std::vector<DynBitset> initial_states;
  DynBitset final_states;
  DynBitset removed_states;
public:
  VWAA (size_t n_ap_, size_t max_state_)
    : n_ap(n_ap_), max_state(max_state_), transitions(),
      initial_states(), final_states(max_state_, false),
      removed_states(max_state_, false) {}
  size_t nextStateId() const;
  size_t addState(transition &&t, bool is_final);
  transition &stateTransition(size_t s);
  const std::vector<DynBitset> &initialStates() const;
  const DynBitset &finalStates() const;
  VWAA &setInitialStates(std::vector<DynBitset> &&initial);
  void show(FILE *fp, const Numbering &map) const;
};

VWAA::transition transitionCompose(const std::vector<const VWAA::transition *> &ts);
VWAA::transition &transitionUnion(VWAA::transition &t1,
                                  VWAA::transition &&t2);

#endif
