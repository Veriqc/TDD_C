#ifndef DEFINITIONS_HPP
#define DEFINITIONS_HPP
#include <complex>
#include <cstdint>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dd {
// integer type used for indexing qubits
// needs to be a signed type to encode -1 as the index for the terminal
// std::int8_t can address up to 128 qubits as [0, ..., 127]


// integer type used for specifying numbers of qubits
using QubitCount = std::make_unsigned<int16_t>::type;

// integer type used for reference counting
// 32bit suffice for a max ref count of around 4 billion

// floating point type to use

// logic radix
static constexpr std::uint8_t RADIX = 2;
// max no. of edges = RADIX^2
static constexpr std::uint8_t NEDGE = RADIX* RADIX;

enum class BasisStates {
  zero,  // NOLINT(readability-identifier-naming)
  one,   // NOLINT(readability-identifier-naming)
  plus,  // NOLINT(readability-identifier-naming)
  minus, // NOLINT(readability-identifier-naming)
  right, // NOLINT(readability-identifier-naming)
  left   // NOLINT(readability-identifier-naming)
};

static constexpr double SQRT2_2 = static_cast<double>(
    0.707106781186547524400844362104849039284835937688474036588L);
static constexpr double PI = static_cast<double>(
    3.141592653589793238462643383279502884197169399375105820974L);
static constexpr double PI_2 = static_cast<double>(
    1.570796326794896619231321691639751442098584699687552910487L);
static constexpr double PI_4 = static_cast<double>(
    0.785398163397448309615660845819875721049292349843776455243L);

using CVec = std::vector<std::complex<double>>;
using CMat = std::vector<CVec>;

// use hash maps for representing sparse vectors of probabilities
using ProbabilityVector = std::unordered_map<std::size_t, double>;

static constexpr std::uint64_t SERIALIZATION_VERSION = 1;

// 64bit mixing hash (from MurmurHash3,
// https://github.com/aappleby/smhasher/blob/master/src/MurmurHash3.cpp)
constexpr std::size_t murmur64(std::size_t k) {
  k ^= k >> 33;
  k *= 0xff51afd7ed558ccdULL;
  k ^= k >> 33;
  k *= 0xc4ceb9fe1a85ec53ULL;
  k ^= k >> 33;
  return k;
}

// combine two 64bit hashes into one 64bit hash (boost::hash_combine,
// https://www.boost.org/LICENSE_1_0.txt)
constexpr std::size_t combineHash(std::size_t lhs, std::size_t rhs) {
  lhs ^= rhs + 0x9e3779b97f4a7c15ULL + (lhs << 6) + (lhs >> 2);
  return lhs;
}

// calculates the Units in Last Place (ULP) distance of two floating point
// numbers
[[maybe_unused]] static std::size_t ulpDistance(double a, double b) {
  // NOLINTNEXTLINE(clang-diagnostic-float-equal)
  if (a == b) {
    return 0;
  }

  std::size_t ulps = 1;
  double nextFP = std::nextafter(a, b);
  // NOLINTNEXTLINE(clang-diagnostic-float-equal)
  while (nextFP != b) {
    ulps++;
    nextFP = std::nextafter(nextFP, b);
  }
  return ulps;
}
} // namespace dd
#endif