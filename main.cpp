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

  size_t n_all;
  size_t n_one;
  fscanf(input, "%lu %lu", &n_all, &n_one);
  for (size_t _ = 0; _ < n_all; _++) {
    fprintf(output, "%d\n",
            productIsEmpty
            (ts,
             BA
             (genGBA
              (genVWAA
               (ap.size(),
                mkNegation
                (parse(input, ap))
                .toNNF())))));
  }
  for (size_t _ = 0; _ < n_one; _++) {
    size_t init;
    fscanf(input, "%lu", &init);
    fprintf(output, "%d\n",
            productIsEmptyFrom
            (ts,
             BA
             (genGBA
              (genVWAA
               (ap.size(),
                mkNegation
                (parse(input, ap))
                .toNNF()))),
             init));
  }
}
