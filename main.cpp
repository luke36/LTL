#include "parse.hpp"
#include "verify.hpp"

int main(int argc, char *argv[]) {
  FILE *input_ts = stdin;
  FILE *input_formula = stdin;
  FILE *output = stdout;
  parseCLIArgs(argc, argv, input_ts, input_formula, output);

  Numbering ap;
  Numbering act;
  TS &&ts = readTS(input_ts, ap, act);

  size_t n_all;
  size_t n_one;
  fscanf(input_formula, "%lu %lu", &n_all, &n_one);
  for (size_t _ = 0; _ < n_all; _++) {
    auto &&ret =
      productIsEmpty
      (ts,
       BA
       (genGBA
        (genVWAA
         (ap.size(),
          mkNegation
          (parse(input_formula, ap))
          .toNNF()))));
    ret.show(output, ap);
    fprintf(output, "\n");
  }
  for (size_t _ = 0; _ < n_one; _++) {
    size_t init;
    fscanf(input_formula, "%lu", &init);
    auto &&ret =
      productIsEmptyFrom
      (ts,
       BA
       (genGBA
        (genVWAA
         (ap.size(),
          mkNegation
          (parse(input_formula, ap))
          .toNNF()))),
       &ts.getState(init));
    ret.show(output, ap);
    fprintf(output, "\n");
  }

  fclose(input_ts);
  if (input_ts != input_formula)
    fclose(input_formula);
  fclose(output);
}
