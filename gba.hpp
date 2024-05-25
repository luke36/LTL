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

    bool operator==(const single_transition &other) const {
      return final_set == other.final_set && t == other.t;
    }

    bool subsume(const single_transition &other) const {
      return
        other.final_set.subset(final_set) && t.subsume(other.t);
    }
  };
  typedef std::list<single_transition> transition;

private:
  std::map<DynBitset, transition> transitions;
  std::map<DynBitset, DynBitset> redirect;
  std::vector<DynBitset> initial;
  size_t n_final;
public:
  GBA(const std::vector<DynBitset> &initial_, size_t n_final_)
    : transitions(), redirect(), initial(initial_), n_final(n_final_) {}

  bool hasState(const DynBitset &state) const {
    return
      transitions.find(state) != transitions.end() ||
      redirect.find(state) != redirect.end();
  }

  GBA &addState(DynBitset &&state, transition &&transition);

  GBA &finalize() {
    redirect.clear();
    return *this;
  }

  const std::map<DynBitset, transition> &allStates() const {
    return transitions;
  }

  const std::vector<DynBitset> &initialStates() const {
    return initial;
  }

  size_t nFinal() const {
    return n_final;
  }

  void show(FILE *fp, const Numbering &map) const;
};

GBA genGBA(const VWAA &aa);

#endif
