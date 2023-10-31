#pragma once

#include <Cir_tn.h>
#include <test.hpp>

namespace test {
	void test_TensorNetwork_cont() {
		std::cout << "test gate to tensor" << std::endl;
		extern void test_gate();
		std::cout << "-------------------" << std::endl;
		std::cout << "test circuit to tn" << std::endl;
		extern void test_tn();
	}
}
void test_gate() {
	dd::Tensor ts = cir_tn::gate_2_tensor("x", { {"x",0},{"y",1} });
	std::cout << "tensor xarray:" << std::endl << ts.data << std::endl;
}

void test_tn() {
    std::string path2 = "Benchmarks/";
    std::string file_name = "tn_test.qasm";
    int n = cir_tn::get_qubits_num(path2 + file_name);
    auto ddpack = std::make_unique<dd::Package<>>(3 * n);
    dd::TensorNetwork tn = cir_tn::cir_2_tn(path2, file_name, ddpack);
    
	dd::TDD tdd = tn.cont(ddpack);
}