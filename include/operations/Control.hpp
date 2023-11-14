#pragma once

#include "../Definitions.hpp"

#include <set>

namespace qc {
struct Control {
  enum class Type : bool { Pos = true, Neg = false };

  int16_t qubit{};
  Type type = Type::Pos;
};

inline bool operator<(const Control& lhs, const Control& rhs) {
  return lhs.qubit < rhs.qubit ||
         (lhs.qubit == rhs.qubit && lhs.type < rhs.type);
}

inline bool operator==(const Control& lhs, const Control& rhs) {
  return lhs.qubit == rhs.qubit && lhs.type == rhs.type;
}

inline bool operator!=(const Control& lhs, const Control& rhs) {
  return !(lhs == rhs);
}

// this allows a set of controls to be indexed by a `int16_t`
struct CompareControl {
  using is_transparent [[maybe_unused]] = void;

  inline bool operator()(const Control& lhs, const Control& rhs) const {
    return lhs < rhs;
  }

  inline bool operator()(int16_t lhs, const Control& rhs) const {
    return lhs < rhs.qubit;
  }

  inline bool operator()(const Control& lhs, int16_t rhs) const {
    return lhs.qubit < rhs;
  }
};
using Controls = std::set<Control, CompareControl>;

inline namespace literals {
// User-defined literals require unsigned long long int
// NOLINTNEXTLINE(google-runtime-int)
inline Control operator""_pc(unsigned long long int q) {
  return {static_cast<int16_t>(q)};
}
// User-defined literals require unsigned long long int
// NOLINTNEXTLINE(google-runtime-int)
inline Control operator""_nc(unsigned long long int q) {
  return {static_cast<int16_t>(q), Control::Type::Neg};
}
} // namespace literals
} // namespace qc
