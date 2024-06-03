#ifndef PARSE_HPP
#define PARSE_HPP

#include "ltl.hpp"
#include "numbering.hpp"

Formula parse(FILE *, const Numbering &map);
void parseCLIArgs(int argc, char *argv[],
                  FILE *&ts, FILE *&formula, FILE *&output);

#endif
