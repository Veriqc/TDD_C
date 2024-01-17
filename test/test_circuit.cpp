#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <ctime>


#include "dd/Export.hpp"
#include <xtensor/xio.hpp>
#include <xtensor/xarray.hpp>
#include "dd/Cir_tn.hpp"

void test_qasm();

int main(){
    std::cout << "test test.qasm" << std::endl;
    test_qasm();
}
void test_qasm() {
    std::string path2 = "Benchmarks/";
    std::string file_name = "test.qasm";
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