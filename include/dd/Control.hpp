#ifndef CONTRPL_HPP
#define CONTRPL_HPP
#include "Definitions.hpp"

#include <set>

namespace dd {
struct Control {
  enum class Type : bool {
    pos = true, // NOLINT(readability-identifier-naming)
    neg = false // NOLINT(readability-identifier-naming)
  };

  int16_t qubit{};
  Type type = Type::pos;
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
// NOLINTNEXTLINE(google-runtime-int) User-defined literals require ull
inline Control operator""_pc(unsigned long long int q) {
  return {static_cast<int16_t>(q)};
}
// NOLINTNEXTLINE(google-runtime-int) User-defined literals require ull
inline Control operator""_nc(unsigned long long int q) {
  return {static_cast<int16_t>(q), Control::Type::neg};
}
} // namespace literals
} // namespace dd
#endif