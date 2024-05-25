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

static const VWAA::transition &
getRef(const std::variant<VWAA::transition, size_t> &x, VWAA &aa) {
  if (x.index() == 0) {
    return std::get<0>(x);
  } else {
    return aa.stateTransition(std::get<1>(x));
  }
}

std::variant<VWAA::transition, size_t>
NNFTrue::addTransition(size_t nap, size_t max, VWAA &aa) {
  VWAA::transition t = {VWAA::single_transition(nap, max)};
  sid = aa.addState(std::move(t), false);
  return sid;
}

std::variant<VWAA::transition, size_t>
NNFFalse::addTransition(size_t, size_t, VWAA &aa) {
  sid = aa.addState({}, false);
  return sid;
}

std::variant<VWAA::transition, size_t>
NNFAtom::addTransition(size_t nap, size_t max, VWAA &aa) {
  VWAA::single_transition tt(nap, max);
  tt.pos.add(id);
  sid = aa.addState({std::move(tt)}, false);
  return sid;
}

std::variant<VWAA::transition, size_t>
NNFNegAtom::addTransition(size_t nap, size_t max, VWAA &aa) {
  VWAA::single_transition tt(nap, max);
  tt.neg.add(id);
  sid = aa.addState({std::move(tt)}, false);
  return sid;
}

std::variant<VWAA::transition, size_t>
NNFConj::addTransition(size_t nap, size_t max, VWAA &aa) {
  auto &&t1 = f1.addTransition(nap, max, aa);
  auto &&t2 = f2.addTransition(nap, max, aa);
  const VWAA::transition
    &p1 = getRef(t1, aa),
    &p2 = getRef(t2, aa);
  return transitionCompose(p1, p2);
}

std::variant<VWAA::transition, size_t>
NNFDisj::addTransition(size_t nap, size_t max, VWAA &aa) {
  auto &&t1 = f1.addTransition(nap, max, aa);
  auto &&t2 = f2.addTransition(nap, max, aa);

  if (t1.index() == 0) {
    auto &&t1_ = std::move(std::get<0>(t1));
    if (t2.index() == 0) {
      auto &t2_ = std::get<0>(t2);
      return transitionUnion(t1_, std::move(t2_));
    } else {
      auto t2_ = getRef(t2, aa);
      return transitionUnion(t1_, std::move(t2_));
    }
  } else {
    auto t1_ = getRef(t1, aa);
    if (t2.index() == 0) {
      auto &t2_ = std::get<0>(t2);
      return transitionUnion(t1_, std::move(t2_));
    } else {
      auto t2_ = getRef(t2, aa);
      return transitionUnion(t1_, std::move(t2_));
    }
  }
}

std::variant<VWAA::transition, size_t>
NNFNext::addTransition(size_t nap, size_t max, VWAA &aa) {
  f.addTransition(nap, max, aa);
  auto &&s = f.dnf(max);
  VWAA::transition t;
  for (auto &ss : s) {
    t.push_back(VWAA::single_transition(DynBitset(nap, false),
                                        DynBitset(nap, false),
                                        std::move(ss)));
  }
  sid = aa.addState(std::move(t), false);
  return sid;
}

static size_t untilAux(VWAA::transition &t1,
                VWAA::transition &t2,
                VWAA &aa) {
  size_t sid = aa.nextStateId();
  for (auto &t : t1) {
    t.to.add(sid);
  }
  transitionUnion(t2, std::move(t1));
  sid = aa.addState(std::move(t2), true);
  return sid;
}

std::variant<VWAA::transition, size_t>
NNFUntil::addTransition(size_t nap, size_t max, VWAA &aa) {
  auto &&t1 = f1.addTransition(nap, max, aa);
  auto &&t2 = f2.addTransition(nap, max, aa);

  if (t1.index() == 0) {
    auto &t1_ = std::get<0>(t1);
    if (t2.index() == 0) {
      auto &t2_ = std::get<0>(t2);
      sid = untilAux(t1_, t2_, aa);
    } else {
      auto t2_ = getRef(t2, aa);
      sid = untilAux(t1_, t2_, aa);
    }
  } else {
    auto t1_ = getRef(t1, aa);
    if (t2.index() == 0) {
      auto &t2_ = std::get<0>(t2);
      sid = untilAux(t1_, t2_, aa);
    } else {
      auto t2_ = getRef(t2, aa);
      sid = untilAux(t1_, t2_, aa);
    }
  }

  return sid;
}

std::variant<VWAA::transition, size_t>
NNFRelease::addTransition(size_t nap, size_t max, VWAA &aa) {
  auto &&t1 = f1.addTransition(nap, max, aa);
  auto &&t2 = f2.addTransition(nap, max, aa);
  auto &p2 = getRef(t2, aa);
  sid = aa.nextStateId();
  VWAA::single_transition tt(nap, max);
  tt.to.add(sid);

  if (t1.index() == 0) {
    auto &t1_ = std::get<0>(t1);
    t1_.push_back(std::move(tt));
    auto t = transitionCompose(t1_, p2);
    sid = aa.addState(std::move(t), false);
  } else {
    VWAA::transition t1_ = getRef(t1, aa);
    t1_.push_back(std::move(tt));
    auto t = transitionCompose(t1_, p2);
    sid = aa.addState(std::move(t), false);
  }
  return sid;
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

