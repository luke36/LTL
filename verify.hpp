#ifndef VERIFY_HPP
#define VERIFY_HPP

#include "buchi.hpp"
#include "ts.hpp"

bool productIsEmptyFrom(const TS &ts, const BA &ba, size_t init);
bool productIsEmpty(const TS &ts, const BA &ba);

#endif
