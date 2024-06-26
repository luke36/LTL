#ifndef NNF_HPP
#define NNF_HPP

#include <memory>
#include <list>
#include <variant>
#include "numbering.hpp"
#include "vwaa.hpp"

class NNFBase {
public:
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const = 0;
  virtual std::list<DynBitset> dnf(size_t max) const = 0;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) = 0;
  virtual void show(FILE *fp, const Numbering &map) const = 0;
  virtual ~NNFBase() = default;
};

class NNF {
  std::shared_ptr<NNFBase> f;
public:
  size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const {
    return f->nTempral(pos, neg);
  }
  std::list<DynBitset> dnf(size_t max) const {
    auto &&ret = f->dnf(max);
    return ret;
  }
  std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) {
    auto &&ret = f->addTransition(nap, max, aa);
    return ret;
  }
  NNF(std::shared_ptr<NNFBase> f_) : f(f_) {}
  void show(FILE *fp, const Numbering &map) const {
    f->show(fp, map);
  }
};

class NNFTrue : public NNFBase {
  size_t sid;
public:
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFTrue() override = default;
};

class NNFFalse : public NNFBase {
  size_t sid;
public:
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFFalse() override = default;
};

class NNFAtom : public NNFBase {
  size_t id;
  size_t sid;
public:
  NNFAtom(size_t id_) : id(id_) {}
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFAtom() override = default;
};

class NNFNegAtom : public NNFBase {
  size_t id;
  size_t sid;
public:
  NNFNegAtom(size_t id_) : id(id_) {}
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFNegAtom() override = default;
};

class NNFConj : public NNFBase {
  NNF f1, f2;
public:
  NNFConj(NNF f1_, NNF f2_) : f1(f1_), f2(f2_) {}
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFConj() override = default;
};

class NNFDisj : public NNFBase {
  NNF f1, f2;
public:
  NNFDisj(NNF f1_, NNF f2_) : f1(f1_), f2(f2_) {}
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFDisj() override = default;
};

class NNFNext : public NNFBase {
  NNF f;
  size_t sid;
public:
  NNFNext(NNF f_) : f(f_) {}
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFNext() override = default;
};

class NNFUntil : public NNFBase {
  NNF f1, f2;
  size_t sid;
public:
  NNFUntil(NNF f1_, NNF f2_) : f1(f1_), f2(f2_) {}
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFUntil() override = default;
};

class NNFRelease : public NNFBase {
  NNF f1, f2;
  size_t sid;
public:
  NNFRelease(NNF f1_, NNF f2_) : f1(f1_), f2(f2_) {}
  virtual size_t nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const override;
  virtual std::list<DynBitset> dnf(size_t max) const override;
  virtual std::variant<VWAA::transition, size_t>
  addTransition(size_t nap, size_t max, VWAA &aa) override;
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFRelease() override = default;
};

inline NNF mkNNFTrue() {
  NNFBase *p = new NNFTrue();
  return std::shared_ptr<NNFBase>(p);
}

inline NNF mkNNFFalse() {
  NNFBase *p = new NNFFalse();
  return std::shared_ptr<NNFBase>(p);
}

inline NNF mkNNFAtom(size_t id) {
  NNFBase *p = new NNFAtom(id);
  return std::shared_ptr<NNFBase>(p);
}

inline NNF mkNNFNegAtom(size_t id) {
  NNFBase *p = new NNFNegAtom(id);
  return std::shared_ptr<NNFBase>(p);
}

inline NNF mkNNFConj(NNF f1, NNF f2) {
  NNFBase *p = new NNFConj(f1, f2);
  return std::shared_ptr<NNFBase>(p);
}

inline NNF mkNNFDisj(NNF f1, NNF f2) {
  NNFBase *p = new NNFDisj(f1, f2);
  return std::shared_ptr<NNFBase>(p);
}

inline NNF mkNNFNext(NNF f) {
  NNFBase *p = new NNFNext(f);
  return std::shared_ptr<NNFBase>(p);
}

inline NNF mkNNFUntil(NNF f1, NNF f2) {
  NNFBase *p = new NNFUntil(f1, f2);
  return std::shared_ptr<NNFBase>(p);
}

inline NNF mkNNFRelease(NNF f1, NNF f2) {
  NNFBase *p = new NNFRelease(f1, f2);
  return std::shared_ptr<NNFBase>(p);
}

VWAA genVWAA(size_t n_atom, NNF f);

#endif
