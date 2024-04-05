#ifndef VERIFY_HPP
#define VERIFY_HPP

#include "buchi.hpp"
#include "ts.hpp"

struct verify_return {
  bool empty;
  std::vector<const TS::state *> finite_fragment;
  std::vector<const TS::state *> loop_fragment;
  verify_return() : empty(true) {}
  verify_return(std::vector<const TS::state *> &&finite,
                std::vector<const TS::state *> &&loop)
    : empty(false),
      finite_fragment(std::move(finite)), loop_fragment(std::move(loop)) {}
  void show(FILE *fp, const Numbering &ap) const;
};

verify_return productIsEmptyFrom(const TS &ts, const BA &ba, const TS::state *init);
verify_return productIsEmpty(const TS &ts, const BA &ba);

#endif
