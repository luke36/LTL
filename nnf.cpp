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

size_t NNFTrue::nTempral(std::set<size_t> &, std::set<size_t> &) const {
  return 0;
}

size_t NNFFalse::nTempral(std::set<size_t> &, std::set<size_t> &) const {
  return 0;
}

size_t NNFAtom::nTempral(std::set<size_t> &pos, std::set<size_t> &) const {
  if (pos.find(id) == pos.end()) {
    pos.insert(id);
    return 1;
  } else {
    return 0;
  }
}

size_t NNFNegAtom::nTempral(std::set<size_t> &, std::set<size_t> &neg) const {
  if (neg.find(id) == neg.end()) {
    neg.insert(id);
    return 1;
  } else {
    return 0;
  }
}

size_t NNFNext::nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const {
  return 1 + f.nTempral(pos, neg);
}

size_t NNFUntil::nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const {
  return 1 + f1.nTempral(pos, neg) + f2.nTempral(pos, neg);
}

size_t NNFRelease::nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const {
  return 1 + f1.nTempral(pos, neg) + f2.nTempral(pos, neg);
}

size_t NNFConj::nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const {
  return f1.nTempral(pos, neg) + f2.nTempral(pos, neg);
}

size_t NNFDisj::nTempral(std::set<size_t> &pos, std::set<size_t> &neg) const {
  return f1.nTempral(pos, neg) + f2.nTempral(pos, neg);
}

std::list<DynBitset> NNFTrue::dnf(size_t max) const {
  return {DynBitset(max, false).add(sid)};
}

std::list<DynBitset> NNFFalse::dnf(size_t max) const {
  return {DynBitset(max, false).add(sid)};
}

std::list<DynBitset> NNFAtom::dnf(size_t max) const {
  return {DynBitset(max, false).add(sid)};
}

std::list<DynBitset> NNFNegAtom::dnf(size_t max) const {
  return {DynBitset(max, false).add(sid)};
}

std::list<DynBitset> NNFNext::dnf(size_t max) const {
  return {DynBitset(max, false).add(sid)};
}

std::list<DynBitset> NNFUntil::dnf(size_t max) const {
  return {DynBitset(max, false).add(sid)};
}

std::list<DynBitset> NNFRelease::dnf(size_t max) const {
  return {DynBitset(max, false).add(sid)};
}

std::list<DynBitset> NNFDisj::dnf(size_t max) const {
  auto &&s1 = f1.dnf(max);
  auto &&s2 = f2.dnf(max);
  s1.splice(s1.end(), s2);
  return s1;
}

std::list<DynBitset> NNFConj::dnf(size_t max) const {
  auto &&s1 = f1.dnf(max);
  auto &&s2 = f2.dnf(max);
  std::list<DynBitset> ret;
  for (auto &s : s1) {
    for (auto &t : s2) {
      auto s_ = s;
      s_.unify(t);
      ret.push_back(std::move(s_));
    }
  }
  return ret;
}

VWAA::transition NNFTrue::addTransition(size_t nap, size_t max, VWAA &aa) {
  VWAA::transition t = {VWAA::single_transition(nap, max)};
  VWAA::transition t_ = t;
  sid = aa.addState(std::move(t_), false);
  return t;
}

VWAA::transition NNFFalse::addTransition(size_t, size_t, VWAA &aa) {
  sid = aa.addState({}, false);
  return {};
}

VWAA::transition NNFAtom::addTransition(size_t nap, size_t max, VWAA &aa) {
  VWAA::single_transition tt(nap, max);
  tt.pos.add(id);
  VWAA::transition t = {std::move(tt)};
  VWAA::transition t_ = t;
  sid = aa.addState(std::move(t_), false);
  return t;
}

VWAA::transition NNFNegAtom::addTransition(size_t nap, size_t max, VWAA &aa) {
  VWAA::single_transition tt(nap, max);
  tt.neg.add(id);
  VWAA::transition t = {std::move(tt)};
  VWAA::transition t_ = t;
  sid = aa.addState(std::move(t_), false);
  return t;
}

VWAA::transition NNFConj::addTransition(size_t nap, size_t max, VWAA &aa) {
  auto &&t1 = f1.addTransition(nap, max, aa);
  auto &&t2 = f2.addTransition(nap, max, aa);
  return transitionCompose({&t1, &t2});
}

VWAA::transition NNFDisj::addTransition(size_t nap, size_t max, VWAA &aa) {
  auto &&t1 = f1.addTransition(nap, max, aa);
  auto &&t2 = f2.addTransition(nap, max, aa);
  return transitionUnion(t1, std::move(t2));
}

VWAA::transition NNFNext::addTransition(size_t nap, size_t max, VWAA &aa) {
  f.addTransition(nap, max, aa);
  auto &&s = f.dnf(max);
  VWAA::transition t;
  for (auto &ss : s) {
    t.push_back(VWAA::single_transition(DynBitset(nap, false),
                                        DynBitset(nap, false),
                                        std::move(ss)));
  }
  VWAA::transition t_ = t;
  sid = aa.addState(std::move(t_), false);
  return t;
}

VWAA::transition NNFUntil::addTransition(size_t nap, size_t max, VWAA &aa) {
  auto &&t1 = f1.addTransition(nap, max, aa);
  auto &&t2 = f2.addTransition(nap, max, aa);
  sid = aa.nextStateId();
  for (auto &t : t1) {
    t.to.add(sid);
  }
  transitionUnion(t2, std::move(t1));
  auto t_ = t2;
  aa.addState(std::move(t_), true);
  return t2;
}

VWAA::transition NNFRelease::addTransition(size_t nap, size_t max, VWAA &aa) {
  auto &&t1 = f1.addTransition(nap, max, aa);
  auto &&t2 = f2.addTransition(nap, max, aa);
  sid = aa.nextStateId();
  VWAA::single_transition tt(nap, max);
  tt.to.add(sid);
  t1.push_back(std::move(tt));
  auto t = transitionCompose({&t1, &t2});
  auto t_ = t;
  aa.addState(std::move(t_), false);
  return t;
}

VWAA genVWAA(size_t n_atom, NNF f) {
  std::set<size_t> pos, neg;
  size_t max = f.nTempral(pos, neg) + 2; // tt, ff
  VWAA ret(n_atom, max);
  f.addTransition(n_atom, max, ret);
  auto &&initial = f.dnf(max);
  std::vector<DynBitset> init;
  for (auto &s : initial) {
    init.emplace_back(std::move(s));
  }
  ret.setInitialStates(std::move(init));
  return ret;
}

