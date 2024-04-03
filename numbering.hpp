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
  bool addString(std::string &&str);
  std::optional<size_t> toId(const std::string &str) const;
  const std::string *toString(size_t id) const;
};

#endif
