#ifndef LTL_HPP
#define LTL_HPP

#include <memory>
#include <cstdio>
#include <string>
#include "numbering.hpp"
#include "nnf.hpp"

class FormulaBase {
public:
  virtual void show(FILE *fp, const Numbering &map) const = 0;
  virtual NNF toNNF() const = 0;
  virtual NNF pushNegation() const = 0;
  virtual ~FormulaBase() = default;
};

class Formula {
  std::shared_ptr<FormulaBase> f;
public:
  Formula(std::shared_ptr<FormulaBase> f_) : f(f_) {}
  void show(FILE *fp, const Numbering &map) const {
    f->show(fp, map);
  }
  NNF toNNF() const {
    return f->toNNF();
  }
  NNF pushNegation() const {
    return f->pushNegation();
  }
};

class FormulaAtom : public FormulaBase {
  const size_t id;
public:
  FormulaAtom(size_t id_)
    : id(id_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaAtom() override = default;
};

class FormulaNegation : public FormulaBase {
  const Formula f;
public:
  FormulaNegation(Formula f_) : f(f_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaNegation() override = default;
};

class FormulaConj : public FormulaBase {
  const Formula f1, f2;
public:
  FormulaConj(Formula f1_, Formula f2_) : f1(f1_), f2(f2_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaConj() override = default;
};

class FormulaDisj : public FormulaBase {
  const Formula f1, f2;
public:
  FormulaDisj(Formula f1_, Formula f2_) : f1(f1_), f2(f2_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaDisj() override = default;
};

class FormulaImply : public FormulaBase {
  const Formula f1, f2;
public:
  FormulaImply(Formula f1_, Formula f2_) : f1(f1_), f2(f2_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaImply() override = default;
};

class FormulaNext : public FormulaBase {
  const Formula f;
public:
  FormulaNext(Formula f_) : f(f_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaNext() override = default;
};

class FormulaAlways : public FormulaBase {
  const Formula f;
public:
  FormulaAlways(Formula f_) : f(f_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaAlways() override = default;
};

class FormulaEvent : public FormulaBase {
  const Formula f;
public:
  FormulaEvent(Formula f_) : f(f_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaEvent() override = default;
};

class FormulaUntil : public FormulaBase {
  const Formula f1, f2;
public:
  FormulaUntil(Formula f1_, Formula f2_) : f1(f1_), f2(f2_) {}
  void show(FILE *fp, const Numbering &map) const override;
  NNF toNNF() const override;
  NNF pushNegation() const override;
  ~FormulaUntil() override = default;
};

inline Formula mkAtom(size_t id) {
  FormulaBase *p = new FormulaAtom(id);
  return std::shared_ptr<FormulaBase>(p);
}

inline Formula mkNegation(Formula f) {
  FormulaBase *p = new FormulaNegation(f);
  return std::shared_ptr<FormulaBase>(p);
}

inline Formula mkConj(Formula f1, Formula f2) {
  FormulaBase *p = new FormulaConj(f1, f2);
  return std::shared_ptr<FormulaBase>(p);
}

inline Formula mkDisj(Formula f1, Formula f2) {
  FormulaBase *p = new FormulaDisj(f1, f2);
  return std::shared_ptr<FormulaBase>(p);
}

inline Formula mkImply(Formula f1, Formula f2) {
  FormulaBase *p = new FormulaImply(f1, f2);
  return std::shared_ptr<FormulaBase>(p);
}

inline Formula mkNext(Formula f) {
  FormulaBase *p = new FormulaNext(f);
  return std::shared_ptr<FormulaBase>(p);
}

inline Formula mkAlways(Formula f) {
  FormulaBase *p = new FormulaAlways(f);
  return std::shared_ptr<FormulaBase>(p);
}

inline Formula mkEvent(Formula f) {
  FormulaBase *p = new FormulaEvent(f);
  return std::shared_ptr<FormulaBase>(p);
}

inline Formula mkUntil(Formula f1, Formula f2) {
  FormulaBase *p = new FormulaUntil(f1, f2);
  return std::shared_ptr<FormulaBase>(p);
}

#endif
