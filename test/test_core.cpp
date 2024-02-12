#include "QuantumComputation.hpp"
#include "dd/Cir_tn.hpp"
// #include "gtest/gtest.h"
#include <filesystem>
#include <iostream>
#include <string>
int main(){
  auto QC = std::make_unique<qc::QuantumComputation>(5);
  // QC.h(2);
  // QC.cx(1,2);
  // QC.s(2);
  // QC.sdg(4);
  // QC.cx(3,4);
  QC->h(0);
  QC->cx(0,1);
  QC->cx(0,2);
  QC->cx(0,3);
  QC->cx(0,4);
  QC->h(0);
  QC->printStatistics(std::cout);
  QC->printPermutation(QC->initialLayout);
  auto ddPack = std::make_unique<dd::Package<>>(QC->getNqubits());
  cir_2_tn(QC,ddPack);
}