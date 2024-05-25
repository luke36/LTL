#ifndef NUMBERING_HPP
#define NUMBERING_HPP

#include <map>
#include <string>
#include <optional>

class Numbering {
  std::map<size_t, std::string> to_str;
  std::map<std::string, size_t> to_id;
  size_t next;
public:
  Numbering()
    : to_str(), to_id(), next(0) {};
  bool addString(std::string &&str) {
    auto ret = to_id.insert({str, next});
    if (!ret.second) {
      return false;
    } else {
      to_str.insert({next, std::move(str)});
      next += 1;
      return true;
    }
  }

  std::optional<size_t> toId(const std::string &str) const {
    auto i = to_id.find(str);
    if (i != to_id.cend()) {
      return i->second;
    } else {
      return {};
    }
  }

  const std::string *toString(size_t id) const {
    auto i = to_str.find(id);
    if (i != to_str.cend()) {
      return &i->second;
    } else {
      return nullptr;
    }
  }

  size_t size() const {
    return next;
  }
};

#endif
