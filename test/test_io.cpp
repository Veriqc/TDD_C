#include "QuantumComputation.hpp"
#include "parsers/qasm3_parser/Exception.hpp"

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
  for(auto& op: *QC){
    std::cout << op->isControlled() << std::endl;
    std::cout << "qubits: " ;
    for(auto& q: op->getUsedQubits()){
      std::cout << q <<" ";
    }
    std::cout << std::endl;
    if(op->isControlled()){
      auto c = op->getControls();
      for(auto i: c){
        std::cout << i.qubit << " ";
      }
      std::cout << std::endl;
    }
    std::cout<< op->getName() << std::endl;
    // op->print(std::cout);
  }
  return 0;
}