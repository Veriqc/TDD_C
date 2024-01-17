#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <ctime>
#include <time.h>
#include <xtensor/xio.hpp>
#include <xtensor/xarray.hpp>
#include "dd/Cir_tn.hpp"
#include <algorithm>

using namespace dd;
TDD cont(dd::TensorNetwork* tn,dd::Package<>* ddpackage, bool release = true) {
    if (!ddpackage) {
        throw std::runtime_error("ddpackage is null");
    }
    if (tn->tensors.size() == 0) {
        throw std::runtime_error("null tensor network");
    }

    clock_t start,end;
    start = clock();
    TDD res_dd = tn->tensors[0].to_tdd(ddpackage);
    ddpackage->incRef(res_dd.e);
    TDD temp_dd;
    unsigned int MAX_NODE = ddpackage->size(res_dd.e);
    
    for (int i=1; i < tn->tensors.size(); ++i) {
        std::cout << i <<std::endl;
        auto tensor = tn->tensors[i];
        try{
            temp_dd = ddpackage->cont(res_dd, tensor.to_tdd(ddpackage));
            if (release) {
                ddpackage->incRef(temp_dd.e);
                ddpackage->decRef(res_dd.e);
                ddpackage->garbageCollect();
            }
            res_dd = temp_dd;
            MAX_NODE = std::max(MAX_NODE, ddpackage->size(res_dd.e));
            export2Dot(res_dd.e,std::to_string(i+10));
        }
        catch(...){
            std::exception_ptr p = std::current_exception();
            std::clog <<(p ? p.__cxa_exception_type()->name() : "null ") << std::endl;
        }
    }
    end = clock();
    std::cout<<"time: " << double(end-start)/CLOCKS_PER_SEC << "s" <<std::endl;
    std::cout<<"MAX node: " << MAX_NODE  <<std::endl;

    return res_dd;
};
int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number>\n";
        return 1;
    }

    std::string path2 = std::string(PROJECT_SOURCE_DIR)+"/Benchmarks/";
    std::string file_name = std::string("bv_") + argv[1] + ".qasm";
	std::cout << path2+file_name << std::endl;
    
    int n = get_qubits_num(path2 + file_name);
    auto ddpack = std::make_unique<dd::Package<>>(3 * n);
    dd::TensorNetwork tn = cir_2_tn(path2, file_name, ddpack.get());

	dd::TDD tdd = cont(&tn,ddpack.get());
    
    std::cout<<"final node: " << ddpack->size(tdd.e) <<std::endl;
    return 0;
}
