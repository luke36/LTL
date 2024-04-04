#include "ltl.hpp"
#include "parse.hpp"
#include "nnf.hpp"
#include "gba.hpp"
#include "buchi.hpp"
#include "ts.hpp"
#include "verify.hpp"
#include <vector>

int main() {
  FILE *input = stdin;
  FILE *output = stdout;
  Numbering ap;
  Numbering act;
  TS &&ts = readTS(input, ap, act);
  ts.show(output, ap, act);

  size_t n_all;
  size_t n_one;
  fscanf(input, "%lu %lu", &n_all, &n_one);
  for (size_t _ = 0; _ < n_all; _++) {
    auto &&ret =
      productIsEmpty
      (ts,
       BA
       (genGBA
        (genVWAA
         (ap.size(),
          mkNegation
          (parse(input, ap))
          .toNNF()))));
    ret.show(output, ts, ap);
    fprintf(output, "\n");
  }
  for (size_t _ = 0; _ < n_one; _++) {
    size_t init;
    fscanf(input, "%lu", &init);
    auto &&ret =
      productIsEmptyFrom
      (ts,
       BA
       (genGBA
        (genVWAA
         (ap.size(),
          mkNegation
          (parse(input, ap))
          .toNNF()))),
       init);
    ret.show(output, ts, ap);
    fprintf(output, "\n");
  }
}
