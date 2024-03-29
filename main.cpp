#include "ltl.hpp"
#include "parse.hpp"

int main() {
  Numbering map;
  map.addString("p");
  map.addString("q");
  char c;
  while ((c = getchar()) != EOF) {
    ungetc(c, stdin);
    Formula f = parse(stdin, map);
    f.show(stdout, map);
    printf("\n  =>  ");
    f.toNNF().show(stdout, map);
    printf("\n");
  }
}
