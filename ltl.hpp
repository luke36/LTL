#ifndef LTL_HPP
#define LTL_HPP

#include <memory>
#include <cstdio>
#include <string>

class FormulaBase {
public:
  virtual void show(FILE *fp) const = 0;
  virtual void pretty(FILE *fp) const = 0;
  virtual ~FormulaBase() {};
};

class Formula {
  const std::shared_ptr<FormulaBase> f;
public:
  Formula(std::shared_ptr<FormulaBase> f_) : f(f_) {}
  void show(FILE *fp) const {
    f->show(fp);
  }
  void pretty(FILE *fp) const {
    f->pretty(fp);
  }
};

class FormulaAtom : public FormulaBase {
  const size_t id;
  const std::string name;
public:
  FormulaAtom(size_t id_, const std::string &name_)
    : id(id_), name(name_) {}
  void show(FILE *fp) const override;
  void pretty(FILE *fp) const override;
  ~FormulaAtom() override = default;
};

class FormulaNegation : public FormulaBase {
  const Formula f;
public:
  FormulaNegation(Formula f_) : f(f_) {}
  void show(FILE *fp) const override;
  void pretty(FILE *fp) const override;
  ~FormulaNegation() override = default;
};

class FormulaConj : public FormulaBase {
  const Formula f1, f2;
public:
  FormulaConj(Formula f1_, Formula f2_) : f1(f1_), f2(f2_) {}
  void show(FILE *fp) const override;
  void pretty(FILE *fp) const override;
  ~FormulaConj() override = default;
};

class FormulaDisj : public FormulaBase {
  const Formula f1, f2;
public:
  FormulaDisj(Formula f1_, Formula f2_) : f1(f1_), f2(f2_) {}
  void show(FILE *fp) const override;
  void pretty(FILE *fp) const override;
  ~FormulaDisj() override = default;
};

class FormulaNext : public FormulaBase {
  const Formula f;
public:
  FormulaNext(Formula f_) : f(f_) {}
  void show(FILE *fp) const override;
  void pretty(FILE *fp) const override;
  ~FormulaNext() override = default;
};

class FormulaAlways : public FormulaBase {
  const Formula f;
public:
  FormulaAlways(Formula f_) : f(f_) {}
  void show(FILE *fp) const override;
  void pretty(FILE *fp) const override;
  ~FormulaAlways() override = default;
};

class FormulaEvent : public FormulaBase {
  const Formula f;
public:
  FormulaEvent(Formula f_) : f(f_) {}
  void show(FILE *fp) const override;
  void pretty(FILE *fp) const override;
  ~FormulaEvent() override = default;
};

class FormulaUntil : public FormulaBase {
  const Formula f1, f2;
public:
  FormulaUntil(Formula f1_, Formula f2_) : f1(f1_), f2(f2_) {}
  void show(FILE *fp) const override;
  void pretty(FILE *fp) const override;
  ~FormulaUntil() override = default;
};

Formula mkAtom(size_t id, const std::string &name);
Formula mkNegation(Formula f);
Formula mkConj(Formula f1, Formula f2);
Formula mkDisj(Formula f1, Formula f2);
Formula mkNext(Formula f);
Formula mkAlways(Formula f);
Formula mkEvent(Formula f);
Formula mkUntil(Formula f1, Formula f2);

#endif
