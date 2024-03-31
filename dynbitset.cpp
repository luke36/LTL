#include "dynbitset.hpp"

size_t DynBitset::size() const {
  return sz;
}

DynBitset &DynBitset::add(size_t x) {
  size_t iblock = x / blockbit;
  size_t i = x % blockbit;
  blocks[iblock] |= ((block_type)1 << i);
  return *this;
}

DynBitset &DynBitset::remove(size_t x) {
  size_t iblock = x / blockbit;
  size_t i = x % blockbit;
  blocks[iblock] &= ~((block_type)1 << i);
  return *this;
}

DynBitset &DynBitset::unify(const DynBitset &other) {
  for (size_t i = 0; i < nblock; i++) {
    blocks[i] |= other.blocks[i];
  }
  return *this;
}

DynBitset &DynBitset::intersect(const DynBitset &other) {
  for (size_t i = 0; i < nblock; i++) {
    blocks[i] &= other.blocks[i];
  }
  return *this;
}

bool DynBitset::has(size_t x) const {
  size_t iblock = x / blockbit;
  size_t i = x % blockbit;
  return (blocks[iblock] & ((block_type)1 << i));
}

bool DynBitset::subset(const DynBitset &other) const {
  for (size_t i = 0; i < nblock; i++) {
    if (blocks[i] & ~other.blocks[i]) {
      return false;
    }
  }
  return true;
}

bool DynBitset::exclude(const DynBitset &other) const {
  for (size_t i = 0; i < nblock; i++) {
    if (blocks[i] & other.blocks[i]) {
      return false;
    }
  }
  return true;
}

bool DynBitset::operator<(const DynBitset &other) const {
  for (size_t i = 0; i < nblock; i++) {
    if (blocks[i] < other.blocks[i]) {
      return true;
    } else if (blocks[i] > other.blocks[i]) {
      return false;
    }
  }
  return false;
}

bool DynBitset::operator==(const DynBitset &other) const {
  for (size_t i = 0; i < nblock; i++) {
    if (blocks[i] != other.blocks[i]) {
      return false;
    }
  }
  return true;
}

