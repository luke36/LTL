#include "ltl.hpp"
#include "parse.hpp"
#include "nnf.hpp"
#include <vector>

int main() {
  Numbering map;
  std::vector<std::string> ap = {"p", "q", "r"};
  for (auto &p : ap) {
    map.addString(p);
  }
  char c;
  while ((c = getchar()) != EOF) {
    ungetc(c, stdin);
    Formula f = parse(stdin, map);
    f.show(stdout, map);
    printf("\n  =>  ");
    NNF nnf = f.toNNF();
    nnf.show(stdout, map);
    printf("\n");
    genVWAA(ap.size(), nnf).show(stdout, map);
  }
}
