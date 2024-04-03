#include "numbering.hpp"

bool Numbering::addString(std::string &&str) {
  auto ret = to_id.insert({str, next});
  if (!ret.second) {
    return false;
  } else {
    to_str.insert({next, std::move(str)});
    next += 1;
    return true;
  }
}

std::optional<size_t> Numbering::toId(const std::string &str) const {
  auto i = to_id.find(str);
  if (i != to_id.cend()) {
    return i->second;
  } else {
    return {};
  }
}

const std::string *Numbering::toString(size_t id) const {
  auto i = to_str.find(id);
  if (i != to_str.cend()) {
    return &i->second;
  } else {
    return nullptr;
  }
}
