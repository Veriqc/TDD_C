#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <ctime>
#include <time.h>
#include "../include/dd/Export.hpp"
#include <xtensor/xio.hpp>
#include <xtensor/xarray.hpp>
#include "../include/Cir_tn.hpp"

void test_bv(int qubits);

int main(){
    for(int i =10; i< 110 ; i = i+10){
        test_bv(i);
    }
}

void test_bv(int qubits) {
    std::string path2 = "/home/gaodc/TDD_C/Benchmarks/";
    std::string file_name = "bv_" + std::to_string(qubits) + ".qasm";
	std::cout << path2+file_name << std::endl;
    clock_t start,end;
    start = clock();
    int n = get_qubits_num(path2 + file_name);
    auto ddpack = std::make_unique<dd::Package<>>(3 * n);
    dd::TensorNetwork tn = cir_2_tn(path2, file_name, ddpack.get());
	tn.infor();
	dd::TDD tdd = tn.cont(ddpack.get());
    end = clock();
    std::cout<<"time: " << double(end-start)/CLOCKS_PER_SEC << "s" <<std::endl;
}