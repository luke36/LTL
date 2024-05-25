#ifndef VWAA_HPP
#define VWAA_HPP

#include "dynbitset.hpp"
#include "numbering.hpp"
#include <set>
#include <vector>
#include <list>

class VWAA {
public:
  struct single_transition {
    DynBitset pos;
    DynBitset neg;
    DynBitset to;
    single_transition(size_t ap, size_t max)
      : pos(ap, false), neg(ap, false), to(max, false) {};
    single_transition(const DynBitset &pos_,
                      const DynBitset &neg_,
                      const DynBitset &to_)
      : pos(pos_), neg(neg_), to(to_) {}
    single_transition(DynBitset &&pos_,
                      DynBitset &&neg_,
                      DynBitset &&to_)
      : pos(std::move(pos_)), neg(std::move(neg_)), to(std::move(to_)) {}
    single_transition(const single_transition &other) = default;
    single_transition(single_transition &&other) = default;
    single_transition &operator=(const single_transition &other) = default;
    single_transition &operator=(single_transition &&other) = default;

    bool operator==(const single_transition &other) const {
      return
        pos == other.pos &&
        neg == other.neg &&
        to == other.to;
    }

    bool subsume(const single_transition &other) const {
      return
        pos.subset(other.pos) &&
        neg.subset(other.neg) &&
        to.subset(other.to);
    }
  };
  typedef std::list<single_transition> transition;

private:
  size_t n_ap;
  std::vector<transition> transitions;
  std::vector<DynBitset> initial_states;
  std::set<size_t> final_states;
  DynBitset removed_states;
public:
  VWAA (size_t n_ap_, size_t max_state)
    : n_ap(n_ap_), transitions(), initial_states(), final_states(),
      removed_states(max_state, false) {}

  size_t MaxState() const {
    return transitions.size();
  }
  size_t nAP() const {
    return n_ap;
  }

  size_t nextStateId() const {
    return transitions.size();
  }

  size_t addState(transition &&t, bool is_final);
  const transition &stateTransition(size_t s) const {
    return transitions[s];
  }

  const std::vector<DynBitset> &initialStates() const {
    return initial_states;
  }

  const std::set<size_t> &finalStates() const {
    return final_states;
  }

  VWAA &setInitialStates(std::vector<DynBitset> &&initial);
  void show(FILE *fp, const Numbering &map) const;
};

std::optional<VWAA::single_transition>
composeSingle(const VWAA::single_transition &t1, const VWAA::single_transition &t2);
void showAP(FILE *fp, const Numbering &map,
            const DynBitset &pos, const DynBitset &neg);
VWAA::transition transitionCompose(const VWAA::transition &t1,
                                   const VWAA::transition &t2);
VWAA::transition &transitionUnion(VWAA::transition &t1,
                                  VWAA::transition &&t2);

#endif
