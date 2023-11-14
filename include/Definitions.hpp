#pragma once

#include "operations/Expression.hpp"

#include <bitset>
#include <deque>
#include <map>
#include <memory>
#include <utility>
#include <variant>
#include <vector>

namespace qc {
class QFRException : public std::invalid_argument {
  std::string msg;

public:
  explicit QFRException(std::string m)
      : std::invalid_argument("QFR Exception"), msg(std::move(m)) {}

  [[nodiscard]] const char* what() const noexcept override {
    return msg.c_str();
  }
};

using Bit = std::uint64_t;

template <class IdxType, class SizeType>
using Register = std::pair<IdxType, SizeType>;
using QuantumRegister = Register<int16_t, std::size_t>;
using ClassicalRegister = Register<Bit, std::size_t>;
template <class RegisterType>
using RegisterMap = std::map<std::string, RegisterType, std::greater<>>;
using QuantumRegisterMap = RegisterMap<QuantumRegister>;
using ClassicalRegisterMap = RegisterMap<ClassicalRegister>;
using RegisterNames = std::vector<std::pair<std::string, std::string>>;

using Targets = std::vector<int16_t>;

using BitString = std::bitset<128>;

// floating-point type used throughout the library

constexpr double PARAMETER_TOLERANCE = 1e-13;

static constexpr double PI = static_cast<double>(
    3.141592653589793238462643383279502884197169399375105820974L);
static constexpr double PI_2 = static_cast<double>(
    1.570796326794896619231321691639751442098584699687552910487L);
static constexpr double PI_4 = static_cast<double>(
    0.785398163397448309615660845819875721049292349843776455243L);

// forward declaration
class Operation;

// supported file formats
enum class Format { Real, OpenQASM, GRCS, TFC, QC, Tensor };

using DAG = std::vector<std::deque<std::unique_ptr<Operation>*>>;
using DAGIterator = std::deque<std::unique_ptr<Operation>*>::iterator;
using DAGReverseIterator =
    std::deque<std::unique_ptr<Operation>*>::reverse_iterator;
using DAGIterators = std::vector<DAGIterator>;
using DAGReverseIterators = std::vector<DAGReverseIterator>;

using Symbolic = sym::Expression<double, double>;
using VariableAssignment = std::unordered_map<sym::Variable, double>;
using SymbolOrNumber = std::variant<Symbolic, double>;
} // namespace qc
