#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <ctime>


#include "dd/Export.hpp"
#include <xtensor/xio.hpp>
#include <xtensor/xarray.hpp>
#include "dd/Cir_tn.hpp"

void test_gate();
void test_tn1();
void test_tn2();

int main(){
	std::cout << "test gate to tensor" << std::endl;
	test_gate();
	std::cout << "-------------------" << std::endl;
	std::cout << "test circuit to tn" << std::endl;
	test_tn1();
	test_tn2();
}

void test_gate() {
	dd::Tensor ts = gate_2_tensor("x", { {"x",0},{"y",1} });
	// std::cout << "tensor xarray:" << std::endl << ts.data << std::endl;
}

void test_tn1() {
    std::string path2 = "/home/gaodc/TDD_C/Benchmarks/";
    std::string file_name = "test_one.qasm";
	std::cout << path2+file_name << std::endl;
    int n = get_qubits_num(path2 + file_name);
	std::cout << "qubits num: " << n << std::endl;
    auto ddpack = std::make_shared<dd::Package<>>(3 * n);
    dd::TensorNetwork tn = cir_2_tn(path2, file_name, ddpack);
	tn.infor();
	ddpack->to_test = true;
	dd::TDD tdd = tn.cont(ddpack);
}

void test_tn2() {
    std::string path2 = "/home/gaodc/TDD_C/Benchmarks/";
    std::string file_name = "test.qasm";
	std::cout << path2+file_name << std::endl;
    int n = get_qubits_num(path2 + file_name);
	std::cout << "qubits num: " << n << std::endl;
    auto ddpack = std::make_shared<dd::Package<>>(3 * n);
    dd::TensorNetwork tn = cir_2_tn(path2, file_name, ddpack);
	tn.infor();
	ddpack->to_test = true;
	dd::TDD tdd = tn.cont(ddpack);
	dd::export2Dot(tdd.e,"qasm_test");
}