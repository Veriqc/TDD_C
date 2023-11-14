#pragma once

#include "Definitions.hpp"
#include "operations/Control.hpp"

#include <map>

namespace qc {
class Permutation : public std::map<int16_t, int16_t> {
public:
  [[nodiscard]] inline Controls apply(const Controls& controls) const {
    Controls c{};
    for (const auto& control : controls) {
      c.emplace(Control{at(control.qubit), control.type});
    }
    return c;
  }
  [[nodiscard]] inline Targets apply(const Targets& targets) const {
    Targets t{};
    for (const auto& target : targets) {
      t.emplace_back(at(target));
    }
    return t;
  }
};
} // namespace qc
