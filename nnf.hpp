#ifndef NNF_HPP
#define NNF_HPP

#include <memory>
#include "numbering.hpp"

class NNFBase {
public:
  virtual void show(FILE *fp, const Numbering &map) const = 0;
  virtual ~NNFBase() = default;
};

class NNF {
  std::shared_ptr<NNFBase> f;
public:
  NNF(std::shared_ptr<NNFBase> f_) : f(f_) {}
  void show(FILE *fp, const Numbering &map) const {
    f->show(fp, map);
  }
};

class NNFTrue : public NNFBase {
public:
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFTrue() override = default;
};

class NNFFalse : public NNFBase {
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFFalse() override = default;
};

class NNFAtom : public NNFBase {
  const size_t id;
public:
  NNFAtom(size_t id_) : id(id_) {}
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFAtom() override = default;
};

class NNFNegAtom : public NNFBase {
  const size_t id;
public:
  NNFNegAtom(size_t id_) : id(id_) {}
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFNegAtom() override = default;
};

class NNFConj : public NNFBase {
  const NNF f1, f2;
public:
  NNFConj(NNF f1_, NNF f2_) : f1(f1_), f2(f2_) {}
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFConj() override = default;
};

class NNFDisj : public NNFBase {
  const NNF f1, f2;
public:
  NNFDisj(NNF f1_, NNF f2_) : f1(f1_), f2(f2_) {}
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFDisj() override = default;
};

class NNFNext : public NNFBase {
  const NNF f;
public:
  NNFNext(NNF f_) : f(f_) {}
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFNext() override = default;
};

class NNFUntil : public NNFBase {
  const NNF f1, f2;
public:
  NNFUntil(NNF f1_, NNF f2_) : f1(f1_), f2(f2_) {}
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFUntil() override = default;
};

class NNFRelease : public NNFBase {
  const NNF f1, f2;
public:
  NNFRelease(NNF f1_, NNF f2_) : f1(f1_), f2(f2_) {}
  virtual void show(FILE *fp, const Numbering &map) const override;
  ~NNFRelease() override = default;
};

NNF mkNNFTrue();
NNF mkNNFFalse();
NNF mkNNFAtom(size_t id);
NNF mkNNFNegAtom(size_t id);
NNF mkNNFConj(NNF f1, NNF f2);
NNF mkNNFDisj(NNF f1, NNF f2);
NNF mkNNFNext(NNF f);
NNF mkNNFUntil(NNF f1, NNF f2);
NNF mkNNFRelease(NNF f1, NNF f2);

#endif
