#include "ltl.hpp"
#include "parse.hpp"

int main() {
  std::map<std::string, size_t> id_map;
  id_map["p"] = 0;
  id_map["q"] = 1;
  parse(stdin, id_map).show(stdout);
}
