#ifndef VERIFY_HPP
#define VERIFY_HPP

#include "buchi.hpp"
#include "ts.hpp"

struct verify_return {
  bool empty;
  std::vector<size_t> finite_fragment;
  std::vector<size_t> loop_fragment;
  verify_return() : empty(true) {}
  verify_return(std::vector<size_t> &&finite, std::vector<size_t> &&loop)
    : empty(false),
      finite_fragment(std::move(finite)), loop_fragment(std::move(loop)) {}
  void show(FILE *fp, const TS &ts, const Numbering &ap) const;
};

verify_return productIsEmptyFrom(const TS &ts, const BA &ba, size_t init);
verify_return productIsEmpty(const TS &ts, const BA &ba);

#endif
