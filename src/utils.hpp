#ifndef UTILS_H
#define UTILS_H

#include <cstdlib>
#include <string>
#include <sstream>
#include <algorithm>

namespace utils {
  void mkdirp(const std::string& name) {
    if (name.empty()) {
      return;
    }
    std::stringstream ss;
    ss << "mkdir -p " << name;
    system(ss.str().c_str());
  }

  std::string path(const std::string& filepath) {
    return filepath.substr(0, filepath.find_last_of("\\/"));
  }

  std::string capitalize(const std::string& word) {
    if (word.empty()) {
      return "";
    }
    std::string capitalized = std::string(word);
    auto& firstChar = capitalized[0];
    firstChar = toupper(firstChar);
    return capitalized;
  }

  std::string upcase(const std::string& s) {
    if (s.empty()) {
      return "";
    }
    std::string uppercased = std::string(s);
    for (auto& c : uppercased) { c = toupper(c); }
    return uppercased;
  }

  std::string basename(const std::string& filename) {
    if (filename.empty()) {
      return "";
    }

    auto len = filename.length();
    auto index = filename.find_last_of("/\\");

    if (index == std::string::npos) {
      return filename;
    }

    if (index + 1 >= len) {
      len -= 1;
      index = filename.substr(0, len).find_last_of("/\\");

      if (len == 0) {
        return filename;
      }

      if (index == 0) {
        return filename.substr(1, len - 1);
      }

      if (index == std::string::npos) {
        return filename.substr(0, len);
      }

      return filename.substr(index + 1, len - index - 1);
    }

    return filename.substr(index + 1, len - index);
  }
}
#endif // !UTILS_H
