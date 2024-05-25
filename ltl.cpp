#include "ltl.hpp"

void FormulaAtom::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "%s", map.toString(id)->c_str());
}

void FormulaNegation::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "! ");
  f.show(fp, map);
}

void FormulaConj::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "(");
  f1.show(fp, map);
  fprintf(fp, " /\\ ");
  f2.show(fp, map);
  fprintf(fp, ")");
}

void FormulaDisj::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "(");
  f1.show(fp, map);
  fprintf(fp, " \\/ ");
  f2.show(fp, map);
  fprintf(fp, ")");
}

void FormulaImply::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "(");
  f1.show(fp, map);
  fprintf(fp, " -> ");
  f2.show(fp, map);
  fprintf(fp, ")");
}

void FormulaNext::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "X ");
  f.show(fp, map);
}

void FormulaAlways::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "G ");
  f.show(fp, map);
}

void FormulaEvent::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "F ");
  f.show(fp, map);
}

void FormulaUntil::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "(");
  f1.show(fp, map);
  fprintf(fp, " U ");
  f2.show(fp, map);
  fprintf(fp, ")");
}

NNF FormulaAtom::toNNF() const {
  return mkNNFAtom(id);
}

NNF FormulaAtom::pushNegation() const {
  return mkNNFNegAtom(id);
}

NNF FormulaNegation::toNNF() const {
  return f.pushNegation();
}

NNF FormulaNegation::pushNegation() const {
  return f.toNNF();
}

NNF FormulaConj::toNNF() const {
  return mkNNFConj(f1.toNNF(), f2.toNNF());
}

NNF FormulaConj::pushNegation() const {
  return mkNNFDisj(f1.pushNegation(), f2.pushNegation());
}

NNF FormulaDisj::toNNF() const {
  return mkNNFDisj(f1.toNNF(), f2.toNNF());
}

NNF FormulaDisj::pushNegation() const {
  return mkNNFConj(f1.pushNegation(), f2.pushNegation());
}

NNF FormulaImply::toNNF() const {
  return mkNNFDisj(f1.pushNegation(), f2.toNNF());
}

NNF FormulaImply::pushNegation() const {
  return mkNNFConj(f1.toNNF(), f2.pushNegation());
}

NNF FormulaNext::toNNF() const {
  return mkNNFNext(f.toNNF());
}

NNF FormulaNext::pushNegation() const {
  return mkNNFNext(f.pushNegation());
}

NNF FormulaAlways::toNNF() const {
  return mkNNFRelease(mkNNFFalse(), f.toNNF());
}

NNF FormulaAlways::pushNegation() const {
  return mkNNFUntil(mkNNFTrue(), f.pushNegation());
}

NNF FormulaEvent::toNNF() const {
  return mkNNFUntil(mkNNFTrue(), f.toNNF());
}

NNF FormulaEvent::pushNegation() const {
  return mkNNFRelease(mkNNFFalse(), f.pushNegation());
}

NNF FormulaUntil::toNNF() const {
  return mkNNFUntil(f1.toNNF(), f2.toNNF());
}

NNF FormulaUntil::pushNegation() const {
  return mkNNFRelease(f1.pushNegation(), f2.pushNegation());
}
