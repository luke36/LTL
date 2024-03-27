#ifndef PARSE_HPP
#define PARSE_HPP

#include "ltl.hpp"
#include <map>

Formula parse(FILE *, std::map<std::string, size_t>);

#endif
