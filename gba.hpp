#ifndef GBA_HPP
#define GBA_HPP

#include "vwaa.hpp"
#include <map>

class GBA {
public:
  struct single_transition {
    DynBitset final_set;
    VWAA::single_transition t;
    single_transition(DynBitset &&final_set_, VWAA::single_transition &&t_)
      : final_set(std::move(final_set_)), t(std::move(t_)) {}
    single_transition(const single_transition &other) = default;
    single_transition(single_transition &&other) = default;
    single_transition &operator=(const single_transition &other) = default;
    single_transition &operator=(single_transition &&other) = default;
    bool operator==(const single_transition &other) const;
    bool subsume(const single_transition &other) const;
  };
  typedef std::list<single_transition> transition;

private:
  std::map<DynBitset, transition> transitions;
  std::map<DynBitset, DynBitset> redirect;
  std::vector<DynBitset> initial;
public:
  GBA(const std::vector<DynBitset> &initial_)
    : transitions(), redirect(), initial(initial_) {}
  bool hasState(const DynBitset &state) const;
  GBA &setInitial(const std::vector<DynBitset> &initial);
  GBA &addState(DynBitset &&state, transition &&transition);
  void show(FILE *fp, const Numbering &map) const;
};

GBA::transition transitionCompose(const std::vector<const VWAA::transition *> ts,
                                  const VWAA &aa);
GBA genGBA(const VWAA &aa);

#endif
