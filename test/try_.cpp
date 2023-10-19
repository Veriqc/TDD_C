#include <iostream>
// #include <xtensor/xarray.hpp>

#include <dd/Package.hpp>
#include <dd/Export.hpp>
int main(int argc, char* argv[])
{
  // dd::ComplexValue one = {1,0} ;
  // dd::ComplexValue zero = {0,0} ;
  // xt::xarray<dd::ComplexValue> U = {{{{one, zero}, {zero, one}}, {{zero, zero}, {zero, zero}}}, {{{zero, zero}, {zero, zero}}, {{zero, one}, {one, zero}}}};
  // std::vector<int> order = {0,1,2,3};
  // xt::xarray<dd::ComplexValue> U = {
  //   {one, zero, zero, zero},
  //   {zero, one, zero, zero},
  //   {zero, zero, zero, one},
  //   {zero, zero, one, zero}};
  // std::vector<dd::Index> indexs = {{"x0",0},{"y0",1},{"x1",2},{"y1",3}}; 
  
  // auto dd1 = std::make_unique<dd::Package<>>(10);
  // dd1->varOrder = {{"x0",0},{"y0",1}};

  // std::vector<dd::Index> indexs ;
  // indexs.push_back(dd::Index({"x0",0}));
  // dd::Tensor tn = {U,indexs};
  // dd::TDD tdd = dd1->Tensor_2_TDD(tn);
  // dd::export2Dot(tdd.e,"tdd1");
  return 0;
}