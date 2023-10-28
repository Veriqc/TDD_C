
#include <iostream>
#include <string>
#include <ctime>
#include "QuantumComputation.hpp"

#include "dd/Export.hpp"
#include "Cir_import.h"
#include <test.hpp>
void test_qasm();

namespace test {
    void test_circuit() {
        std::cout << "test test.qasm" << std::endl;
        test_qasm();
    }
}
void test_qasm() {
    string path2 = "Benchmarks/";
    string file_name = "test.qasm";
    std::array<int,2> nodes;
    int n = get_qubits_num(path2 + file_name);
    auto dd = std::make_unique<dd::Package<>>(3 * n);
    clock_t   start_t, finish_t;
    double time_t;
    std::cout << "File name:" << file_name << std::endl;
    start_t = clock();
    nodes = Simulate_with_tdd(path2, file_name,dd);
    finish_t = clock();
    time_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
    std::cout << "Time:" << time_t << std::endl;
    std::cout << "Nodes max:" << nodes[0] << std::endl;
    std::cout << "Nodes Final:" << nodes[1] << std::endl;
    std::cout << "===================================" << std::endl;
}