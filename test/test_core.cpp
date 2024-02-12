#include "QuantumComputation.hpp"
#include "dd/Cir_tn.hpp"
// #include "gtest/gtest.h"
#include <filesystem>
#include <iostream>
#include <string>
int main(){
    const std::string filePath = std::string(PROJECT_SOURCE_DIR)+"/Benchmark/combinational/bv/bv_5.qasm";

    // Open the file
    std::ifstream fileStream(filePath);
    if (!fileStream.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return -1; // or handle error appropriately
    }

    // Read the file content into a string
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    std::string fileContent = buffer.str();

    // Close the file (optional here since the ifstream will close itself on destruction)
    fileStream.close();

    // Use the file content with QuantumComputation::fromQASM
    const auto qc = qc::QuantumComputation::fromQASM(fileContent);
    std::unique_ptr<qc::QuantumComputation> QC = std::make_unique<qc::QuantumComputation>(std::move(qc));
    auto ddPack = std::make_unique<dd::Package<>>(QC->getNqubits());
    cir_2_tn(QC,ddPack);
}