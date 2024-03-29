#include "nnf.hpp"

void NNFTrue::show(FILE *fp, const Numbering &) const {
  fprintf(fp, "tt");
}

void NNFFalse::show(FILE *fp, const Numbering &) const {
  fprintf(fp, "ff");
}

void NNFAtom::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "%s", map.toString(id)->c_str());
}

void NNFNegAtom::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "~%s", map.toString(id)->c_str());
}

void NNFConj::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "(");
  f1.show(fp, map);
  fprintf(fp, " /\\ ");
  f2.show(fp, map);
  fprintf(fp, ")");
}

void NNFDisj::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "(");
  f1.show(fp, map);
  fprintf(fp, " \\/ ");
  f2.show(fp, map);
  fprintf(fp, ")");
}

void NNFNext::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "X ");
  f.show(fp, map);
}

void NNFUntil::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "(");
  f1.show(fp, map);
  fprintf(fp, " U ");
  f2.show(fp, map);
  fprintf(fp, ")");
}

void NNFRelease::show(FILE *fp, const Numbering &map) const {
  fprintf(fp, "(");
  f1.show(fp, map);
  fprintf(fp, " R ");
  f2.show(fp, map);
  fprintf(fp, ")");
}

using std::shared_ptr;

NNF mkNNFTrue() {
  NNFBase *p = new NNFTrue();
  return shared_ptr<NNFBase>(p);
}

NNF mkNNFFalse() {
  NNFBase *p = new NNFFalse();
  return shared_ptr<NNFBase>(p);
}

NNF mkNNFAtom(size_t id) {
  NNFBase *p = new NNFAtom(id);
  return shared_ptr<NNFBase>(p);
}

NNF mkNNFNegAtom(size_t id) {
  NNFBase *p = new NNFNegAtom(id);
  return shared_ptr<NNFBase>(p);
}

NNF mkNNFConj(NNF f1, NNF f2) {
  NNFBase *p = new NNFConj(f1, f2);
  return shared_ptr<NNFBase>(p);
}

NNF mkNNFDisj(NNF f1, NNF f2) {
  NNFBase *p = new NNFDisj(f1, f2);
  return shared_ptr<NNFBase>(p);
}

NNF mkNNFNext(NNF f) {
  NNFBase *p = new NNFNext(f);
  return shared_ptr<NNFBase>(p);
}

NNF mkNNFUntil(NNF f1, NNF f2) {
  NNFBase *p = new NNFUntil(f1, f2);
  return shared_ptr<NNFBase>(p);
}

NNF mkNNFRelease(NNF f1, NNF f2) {
  NNFBase *p = new NNFRelease(f1, f2);
  return shared_ptr<NNFBase>(p);
}
