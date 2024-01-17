#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <ctime>


#include "dd/Export.hpp"
#include <xtensor/xio.hpp>
#include <xtensor/xarray.hpp>
#include "dd/Cir_tn.hpp"


void tensor_2_tdd_cx_4();
void tensor_2_tdd_cx_2();
void tensor_hyper_index();
int main(){
    std::cout << "test order of 4*4 CNOT" << std::endl;
    tensor_2_tdd_cx_4();
    std::cout << "test order of 2*2*2*2 CNOT" << std::endl;
    tensor_2_tdd_cx_2();
    std::cout << "test hyper CNOT" << std::endl;
    tensor_hyper_index();
}
dd::ComplexValue one = { 1,0 };
dd::ComplexValue zero = { 0,0 };
void tensor_hyper_index(){
    auto dd1 = std::make_unique<dd::Package<>>(10);
    dd1->varOrder = { {"x0",0},{"y0",1},{"x1",2},{"y1",3} };

    xt::xarray<dd::ComplexValue> U = {
        {{{one, zero}, {zero, one}}, {{zero, zero}, {zero, zero}}}, 
        {{{zero, zero}, {zero, zero}}, {{zero, one}, {one, zero}}}
    };
    std::vector<dd::Index> indexs = {{"x0",0},{"x0",1},{"x1",0},{"y1",0}, };

    dd::Tensor tn = { U,indexs, "cx" };
    dd::TDD tdd = tn.to_tdd(dd1.get());
    std::cout << tdd.e.p->v << std::endl;
    dd::export2Dot(tdd.e, "cnot_hyber");
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
    // dd::Tensor tn = {U,indexs, "cx"};
    dd::Tensor tn(U,indexs);
    dd::TDD tdd = tn.to_tdd(dd1.get());
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
    std::vector<dd::Index> indexs = {{"x1",2},{"y1",3}, {"x0",0},{"y0",1}};

    dd::Tensor tn = { U,indexs, "cx" };
    dd::TDD tdd = tn.to_tdd(dd1.get());
    std::cout << tdd.e.p->v << std::endl;
    dd::export2Dot(tdd.e, "cnot_2_2_2_2");
}