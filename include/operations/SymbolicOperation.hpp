#pragma once

#include "../Definitions.hpp"
#include "Expression.hpp"
#include "StandardOperation.hpp"

#include <algorithm>
#include <cstddef>
#include <optional>
#include <unordered_set>
#include <variant>

namespace qc {
// Overload pattern for std::visit
template <typename... Ts> struct Overload : Ts... {
  using Ts::operator()...;
};
template <class... Ts> Overload(Ts...) -> Overload<Ts...>;

class SymbolicOperation final : public StandardOperation {
protected:
  std::vector<std::optional<Symbolic>> symbolicParameter{};

  static OpType parseU3(const Symbolic& theta, double& phi, double& lambda);
  static OpType parseU3(double& theta, const Symbolic& phi, double& lambda);
  static OpType parseU3(double& theta, double& phi, const Symbolic& lambda);
  static OpType parseU3(const Symbolic& theta, const Symbolic& phi, double& lambda);
  static OpType parseU3(const Symbolic& theta, double& phi, const Symbolic& lambda);
  static OpType parseU3(double& theta, const Symbolic& phi, const Symbolic& lambda);

  static OpType parseU2(const Symbolic& phi, const Symbolic& lambda);
  static OpType parseU2(const Symbolic& phi, double& lambda);
  static OpType parseU2(double& phi, const Symbolic& lambda);

  static OpType parseU1(const Symbolic& lambda);

  void checkSymbolicUgate();

  void storeSymbolOrNumber(const SymbolOrNumber& param, std::size_t i);

  [[nodiscard]] bool isSymbolicParameter(const std::size_t i) const {
    return symbolicParameter.at(i).has_value();
  }

  static bool isSymbol(const SymbolOrNumber& param) {
    return std::holds_alternative<Symbolic>(param);
  }

  static Symbolic& getSymbol(SymbolOrNumber& param) {
    return std::get<Symbolic>(param);
  }

  static double& getNumber(SymbolOrNumber& param) { return std::get<double>(param); }

  void setup(std::size_t nq, const std::vector<SymbolOrNumber>& params,
             int16_t startingQubit = 0);

  [[nodiscard]] static double
  getInstantiation(const SymbolOrNumber& symOrNum,
                   const VariableAssignment& assignment);

public:
  SymbolicOperation() = default;

  [[nodiscard]] SymbolOrNumber getParameter(const std::size_t i) const {
    const auto& param = symbolicParameter.at(i);
    if (param.has_value()) {
      return *param;
    }
    return parameter.at(i);
  }

  [[nodiscard]] std::vector<SymbolOrNumber> getParameters() const {
    std::vector<SymbolOrNumber> params{};
    for (std::size_t i = 0; i < parameter.size(); ++i) {
      params.emplace_back(getParameter(i));
    }
    return params;
  }

  void setSymbolicParameter(const Symbolic& par, const std::size_t i) {
    symbolicParameter.at(i) = par;
  }

  // Standard Constructors
  SymbolicOperation(std::size_t nq, int16_t target, OpType g,
                    const std::vector<SymbolOrNumber>& params = {},
                    int16_t startingQubit = 0);
  SymbolicOperation(std::size_t nq, const Targets& targ, OpType g,
                    const std::vector<SymbolOrNumber>& params = {},
                    int16_t startingQubit = 0);

  SymbolicOperation(std::size_t nq, Control control, int16_t target, OpType g,
                    const std::vector<SymbolOrNumber>& params = {},
                    int16_t startingQubit = 0);
  SymbolicOperation(std::size_t nq, Control control, const Targets& targ,
                    OpType g, const std::vector<SymbolOrNumber>& params = {},
                    int16_t startingQubit = 0);

  SymbolicOperation(std::size_t nq, const Controls& c, int16_t target, OpType g,
                    const std::vector<SymbolOrNumber>& params = {},
                    int16_t startingQubit = 0);
  SymbolicOperation(std::size_t nq, const Controls& c, const Targets& targ,
                    OpType g, const std::vector<SymbolOrNumber>& params = {},
                    int16_t startingQubit = 0);

  // MCF (cSWAP), Peres, parameterized two target Constructor
  SymbolicOperation(std::size_t nq, const Controls& c, int16_t target0,
                    int16_t target1, OpType g,
                    const std::vector<SymbolOrNumber>& params = {},
                    int16_t startingQubit = 0);

  [[nodiscard]] std::unique_ptr<Operation> clone() const override {
    return std::make_unique<SymbolicOperation>(
        getNqubits(), getControls(), getTargets(), getType(), getParameters(),
        getStartingQubit());
  }

  [[nodiscard]] inline bool isSymbolicOperation() const override {
    return true;
  }

  [[nodiscard]] inline bool isStandardOperation() const override {
    return std::all_of(symbolicParameter.begin(), symbolicParameter.end(),
                       [](const auto& sym) { return !sym.has_value(); });
  }

  [[nodiscard]] bool equals(const Operation& op, const Permutation& perm1,
                            const Permutation& perm2) const override;
  [[nodiscard]] inline bool equals(const Operation& op) const override {
    return equals(op, {}, {});
  }

  [[noreturn]] void dumpOpenQASM(std::ostream& of, const RegisterNames& qreg,
                                 const RegisterNames& creg) const override;

  [[nodiscard]] StandardOperation
  getInstantiatedOperation(const VariableAssignment& assignment) const;

  // Instantiates this Operation
  // Afterwards casting to StandardOperation can be done if assignment is total
  void instantiate(const VariableAssignment& assignment);
};
} // namespace qc
