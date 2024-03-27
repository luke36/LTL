#ifndef UTIL_HPP
#define UTIL_HPP

#include "cstdio"

#define fail(...)                                                       \
  {                                                                     \
    fprintf(stderr, __VA_ARGS__);                                       \
    exit(1);                                                            \
  }

#endif
