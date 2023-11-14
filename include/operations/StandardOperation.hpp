#pragma once

#include "Operation.hpp"

namespace qc {
class StandardOperation : public Operation {
protected:
  static void checkInteger(double& ld) {
    const double nearest = std::nearbyint(ld);
    if (std::abs(ld - nearest) < PARAMETER_TOLERANCE) {
      ld = nearest;
    }
  }

  static void checkFractionPi(double& ld) {
    const double div = PI / ld;
    const double nearest = std::nearbyint(div);
    if (std::abs(div - nearest) < PARAMETER_TOLERANCE) {
      ld = PI / nearest;
    }
  }

  OpType parseU3(double& theta, double& phi, double& lambda);
  OpType parseU2(double& phi, double& lambda);
  OpType parseU1(double& lambda);

  void checkUgate();
  void setup(std::size_t nq, int16_t startingQubit = 0);

  void dumpOpenQASMTeleportation(std::ostream& of,
                                 const RegisterNames& qreg) const;

public:
  StandardOperation() = default;

  // Standard Constructors
  StandardOperation(std::size_t nq, int16_t target, OpType g,
                    std::vector<double> params = {}, int16_t startingQubit = 0);
  StandardOperation(std::size_t nq, const Targets& targ, OpType g,
                    std::vector<double> params = {}, int16_t startingQubit = 0);

  StandardOperation(std::size_t nq, Control control, int16_t target, OpType g,
                    const std::vector<double>& params = {},
                    int16_t startingQubit = 0);
  StandardOperation(std::size_t nq, Control control, const Targets& targ,
                    OpType g, const std::vector<double>& params = {},
                    int16_t startingQubit = 0);

  StandardOperation(std::size_t nq, const Controls& c, int16_t target, OpType g,
                    const std::vector<double>& params = {},
                    int16_t startingQubit = 0);
  StandardOperation(std::size_t nq, const Controls& c, const Targets& targ,
                    OpType g, const std::vector<double>& params = {},
                    int16_t startingQubit = 0);

  // MCT Constructor
  StandardOperation(std::size_t nq, const Controls& c, int16_t target,
                    int16_t startingQubit = 0);

  // MCF (cSWAP), Peres, parameterized two target Constructor
  StandardOperation(std::size_t nq, const Controls& c, int16_t target0,
                    int16_t target1, OpType g, const std::vector<double>& params = {},
                    int16_t startingQubit = 0);

  [[nodiscard]] std::unique_ptr<Operation> clone() const override {
    return std::make_unique<StandardOperation>(
        getNqubits(), getControls(), getTargets(), getType(), getParameter(),
        getStartingQubit());
  }

  [[nodiscard]] bool isStandardOperation() const override { return true; }

  [[nodiscard]] bool equals(const Operation& op, const Permutation& perm1,
                            const Permutation& perm2) const override {
    return Operation::equals(op, perm1, perm2);
  }
  [[nodiscard]] bool equals(const Operation& operation) const override {
    return equals(operation, {}, {});
  }

  void dumpOpenQASM(std::ostream& of, const RegisterNames& qreg,
                    const RegisterNames& creg) const override;
};

} // namespace qc
