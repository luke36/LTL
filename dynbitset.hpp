#ifndef DYNBITSET_HPP
#define DYNBITSET_HPP

#include <cstddef>

class DynBitset {
  typedef unsigned int block_type;
  static constexpr size_t blockbit = 8 * sizeof(block_type);
  size_t sz;
  size_t nblock;
  block_type *blocks;
public:
  DynBitset(size_t sz_, bool one)
    : sz(sz_),
      nblock(sz_ == 0 ? 0 : (sz_ - 1) / blockbit + 1),
      blocks(sz_ == 0 ? nullptr : new block_type[nblock]) {
    if (sz == 0) {
      return;
    } else if (one) {
      size_t rem = sz % blockbit;
      if (rem == 0) {
        rem = blockbit;
      }
      rem = blockbit - rem;
      blocks[nblock - 1] = ((block_type)(-1) >> rem);
      for (size_t i = 0; i < nblock - 1; i++) {
        blocks[i] = (block_type)(-1);
      }
    } else {
      for (size_t i = 0; i < nblock; i++) {
        blocks[i] = 0;
      }
    }
  }
  DynBitset(size_t sz_, size_t single)
    : sz(sz_),
      nblock((sz_ - 1) / blockbit + 1),
      blocks(new block_type[nblock]) {
    size_t iblock = single / blockbit;
    size_t i = single % blockbit;
    blocks[iblock] = ((block_type)1 << i);
  }
  DynBitset(const DynBitset &other)
    : sz(other.sz),
      nblock(other.nblock),
      blocks(other.sz == 0 ? nullptr : new block_type[nblock]) {
    for (size_t i = 0; i < nblock; i++) {
      blocks[i] = other.blocks[i];
    }
  }
  DynBitset(DynBitset &&other)
    : sz(other.sz),
      nblock(other.nblock),
      blocks(other.blocks) {
    other.blocks = nullptr;
  }
  DynBitset &operator=(const DynBitset &other) {
    if (&other != this) {
      delete [] blocks;
      sz = other.sz;
      nblock = other.nblock;
      blocks = new block_type[nblock];
      for (size_t i = 0; i < nblock; i++) {
        blocks[i] = other.blocks[i];
      }
    }
    return *this;
  }
  DynBitset &operator=(DynBitset &&other) {
    if (&other != this) {
      delete [] blocks;
      sz = other.sz;
      nblock = other.nblock;
      blocks = other.blocks;
      other.blocks = nullptr;
    }
    return *this;
  }
  ~DynBitset() {
    delete [] blocks;
  }

  size_t size() const {
    return sz;
  }

  DynBitset &add(size_t x) {
    size_t iblock = x / blockbit;
    size_t i = x % blockbit;
    blocks[iblock] |= ((block_type)1 << i);
    return *this;
  }

  DynBitset &remove(size_t x) {
    size_t iblock = x / blockbit;
    size_t i = x % blockbit;
    blocks[iblock] &= ~((block_type)1 << i);
    return *this;
  }

  DynBitset &unify(const DynBitset &other) {
    for (size_t i = 0; i < nblock; i++) {
      blocks[i] |= other.blocks[i];
    }
    return *this;
  }

  DynBitset &intersect(const DynBitset &other) {
    for (size_t i = 0; i < nblock; i++) {
      blocks[i] &= other.blocks[i];
    }
    return *this;
  }

  bool has(size_t x) const {
    size_t iblock = x / blockbit;
    size_t i = x % blockbit;
    return (blocks[iblock] & ((block_type)1 << i));
  }

  bool subset(const DynBitset &other) const {
    for (size_t i = 0; i < nblock; i++) {
      if (blocks[i] & ~other.blocks[i]) {
        return false;
      }
    }
    return true;
  }

  bool exclude(const DynBitset &other) const {
    for (size_t i = 0; i < nblock; i++) {
      if (blocks[i] & other.blocks[i]) {
        return false;
      }
    }
    return true;
  }

  bool operator<(const DynBitset &other) const{
    for (size_t i = 0; i < nblock; i++) {
      if (blocks[i] < other.blocks[i]) {
        return true;
      } else if (blocks[i] > other.blocks[i]) {
        return false;
      }
    }
    return false;
  }

  bool operator==(const DynBitset &other) const {
    for (size_t i = 0; i < nblock; i++) {
      if (blocks[i] != other.blocks[i]) {
        return false;
      }
    }
    return true;
  }
};

#endif
