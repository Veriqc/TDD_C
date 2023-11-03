#pragma once

#include "test.hpp"
void test_gate();
void test_tn();
namespace test {
	void test_TensorNetwork_cont() {
		std::cout << "test gate to tensor" << std::endl;
		test_gate();
		std::cout << "-------------------" << std::endl;
		std::cout << "test circuit to tn" << std::endl;
		test_tn();
	}
}
void test_gate() {
	dd::Tensor ts = gate_2_tensor("x", { {"x",0},{"y",1} });
	std::cout << "tensor xarray:" << std::endl << ts.data << std::endl;
}

void test_tn() {
    std::string path2 = "../include/Benchmarks/";
    std::string file_name = "test_one.qasm";
    int n = get_qubits_num(path2 + file_name);
	std::cout << "qubits num: " << n << std::endl;
    auto ddpack = std::make_unique<dd::Package<>>(3 * n);
    dd::TensorNetwork tn = cir_2_tn(path2, file_name, ddpack);
	tn.infor();
	dd::TDD tdd = tn.cont(ddpack);
}