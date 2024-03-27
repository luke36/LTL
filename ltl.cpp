#include "ltl.hpp"

void FormulaAtom::show(FILE *fp) const {
  fprintf(fp, "%s", name.c_str());
}

void FormulaNegation::show(FILE *fp) const {
  fprintf(fp, "! ");
  f.show(fp);
}

void FormulaConj::show(FILE *fp) const {
  fprintf(fp, "(");
  f1.show(fp);
  fprintf(fp, " /\\ ");
  f2.show(fp);
  fprintf(fp, ")");
}

void FormulaDisj::show(FILE *fp) const {
  fprintf(fp, "(");
  f1.show(fp);
  fprintf(fp, " \\/ ");
  f2.show(fp);
  fprintf(fp, ")");
}

void FormulaNext::show(FILE *fp) const {
  fprintf(fp, "X ");
  f.show(fp);
}

void FormulaAlways::show(FILE *fp) const {
  fprintf(fp, "G ");
  f.show(fp);
}

void FormulaEvent::show(FILE *fp) const {
  fprintf(fp, "F ");
  f.show(fp);
}

void FormulaUntil::show(FILE *fp) const {
  fprintf(fp, "(");
  f1.show(fp);
  fprintf(fp, " U ");
  f2.show(fp);
  fprintf(fp, ")");
}

void FormulaAtom::pretty(FILE *fp) const {
  fprintf(fp, "%s", name.c_str());
}

void FormulaNegation::pretty(FILE *fp) const {
  fprintf(fp, "¬");
  f.pretty(fp);
}

void FormulaConj::pretty(FILE *fp) const {
  fprintf(fp, "(");
  f1.pretty(fp);
  fprintf(fp, " ∧ ");
  f2.pretty(fp);
  fprintf(fp, ")");
}

void FormulaDisj::pretty(FILE *fp) const {
  fprintf(fp, "(");
  f1.pretty(fp);
  fprintf(fp, " ∨ ");
  f2.pretty(fp);
  fprintf(fp, ")");
}

void FormulaNext::pretty(FILE *fp) const {
  fprintf(fp, "○");
  f.pretty(fp);
}

void FormulaAlways::pretty(FILE *fp) const {
  fprintf(fp, "□");
  f.pretty(fp);
}

void FormulaEvent::pretty(FILE *fp) const {
  fprintf(fp, "◇");
  f.pretty(fp);
}

void FormulaUntil::pretty(FILE *fp) const {
  fprintf(fp, "(");
  f1.pretty(fp);
  fprintf(fp, " ∪ ");
  f2.pretty(fp);
  fprintf(fp, ")");
}

using std::shared_ptr;

Formula mkAtom(size_t id, const std::string &name) {
  FormulaBase *f = new FormulaAtom(id, name);
  return shared_ptr<FormulaBase>(f);
}

Formula mkNegation(Formula f) {
  FormulaBase *p = new FormulaNegation(f);
  return shared_ptr<FormulaBase>(p);
}

Formula mkConj(Formula f1, Formula f2) {
  FormulaBase *p = new FormulaConj(f1, f2);
  return shared_ptr<FormulaBase>(p);
}

Formula mkDisj(Formula f1, Formula f2) {
  FormulaBase *p = new FormulaDisj(f1, f2);
  return shared_ptr<FormulaBase>(p);
}

Formula mkNext(Formula f) {
  FormulaBase *p = new FormulaNext(f);
  return shared_ptr<FormulaBase>(p);
}

Formula mkAlways(Formula f) {
  FormulaBase *p = new FormulaAlways(f);
  return shared_ptr<FormulaBase>(p);
}

Formula mkEvent(Formula f) {
  FormulaBase *p = new FormulaEvent(f);
  return shared_ptr<FormulaBase>(p);
}

Formula mkUntil(Formula f1, Formula f2) {
  FormulaBase *p = new FormulaUntil(f1, f2);
  return shared_ptr<FormulaBase>(p);
}
