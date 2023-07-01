#include "DDpackage.h"
#include <iostream>
#include <cmath>
#include <string> 
#include "Cir_import.h"

//namespace dd = dd_package;

int main() {


    std::string path = "Benchmarks/";
    std::string file_name = "test.qasm";

    int* nodes;
    int n = get_qubits_num(path + file_name);
    clock_t   start, finish;
    double time;
    std::cout << "File name:" << file_name << std::endl;
    start = clock();
    nodes = Simulate_with_tdd(path, file_name);
    finish = clock();
    time = (double)(finish - start) / CLOCKS_PER_SEC;

    std::cout << "Time:" << time << std::endl;
    std::cout << "Nodes max:" << *nodes << std::endl;
    std::cout << "Nodes Final:" << *(nodes + 1) << std::endl;
    //std::cout << "TDD Nodes:" << *(nodes + 2) << std::endl;

}