#include <iostream>
#include <string>
#include <ctime>
#include <dd/Package.hpp>
#include "dd/Export.hpp"
#include <test.hpp>

dd::ComplexValue one = { 1,0 };
dd::ComplexValue zero = { 0,0 };
void tensor_2_tdd_cx_4();
void tensor_2_tdd_cx_2();
namespace test{
    void test_tdd_2_tensor() {
        std::cout << "test order of 4*4 CNOT" << std::endl;
        tensor_2_tdd_cx_4();
        std::cout << "test order of 2*2*2*2 CNOT" << std::endl;
        tensor_2_tdd_cx_2();
    }
}

void tensor_2_tdd_cx_4(){
    auto dd1 = std::make_unique<dd::Package<>>(10);
    dd1->varOrder = { {"x0",0},{"y0",1},{"x1",2},{"y1",3} };
    xt::xarray<dd::ComplexValue> U = {
        {one, zero, zero, zero},
        {zero, one, zero, zero},
        {zero, zero, zero, one},
        {zero, zero, one, zero}
    };

    std::vector<dd::Index> indexs ={{"y2",3},{"x0",1} };
    dd::Tensor tn = {U,indexs, "cx"};
    dd::TDD tdd = dd1->Tensor_2_TDD(tn);
    std::cout << tdd.e.p->v << std::endl;
    dd::export2Dot(tdd.e,"cnot_4_4");
}
void tensor_2_tdd_cx_2() {
    auto dd1 = std::make_unique<dd::Package<>>(10);
    dd1->varOrder = { {"x0",0},{"y0",1},{"x1",5},{"y1",6} };

    xt::xarray<dd::ComplexValue> U = {
        {{{one, zero}, {zero, one}}, {{zero, zero}, {zero, zero}}}, 
        {{{zero, zero}, {zero, zero}}, {{zero, one}, {one, zero}}}
    };
    std::vector<dd::Index> indexs = {{"x1",2},{"y1",3},{"x0",0},{"y0",1}, };

    dd::Tensor tn = { U,indexs, "cx" };
    dd::TDD tdd = dd1->Tensor_2_TDD(tn);
    std::cout << tdd.e.p->v << std::endl;
    dd::export2Dot(tdd.e, "cnot_2_2_2_2");
}