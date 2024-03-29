#ifndef PARSE_HPP
#define PARSE_HPP

#include "ltl.hpp"
#include "numbering.hpp"
#include <map>

Formula parse(FILE *, const Numbering &map);

#endif
