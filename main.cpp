#include "ltl.hpp"
#include "parse.hpp"
#include "nnf.hpp"
#include "gba.hpp"
#include <vector>

int main() {
  Numbering map;
  std::vector<std::string> ap = {"p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
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
    VWAA &&aa = genVWAA(ap.size(), nnf);
    aa.show(stdout, map);
    GBA &&ba = genGBA(aa);
    ba.show(stdout, map);
  }
}
