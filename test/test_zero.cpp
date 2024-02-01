#include <map>
#include <string>
#include <iostream>
#include <functional>
#include <ctime>


#include "dd/Export.hpp"
#include <xtensor/xio.hpp>
#include <xtensor/xarray.hpp>
#include "dd/Cir_tn.hpp"

using namespace dd;

int main() {
    auto dd1 = std::make_unique<dd::Package<>>(10);
    dd1->varOrder = { {"x0",0},{"y0",1},{"x1",2},{"y1",3} };
    dd::ComplexValue one = { 1,0 };
    dd::ComplexValue zero = { 0,0 };
    xt::xarray<dd::ComplexValue> U = {one,zero};
    std::vector<dd::Index> indexs = {{"x0",0} };

    dd::Tensor tn = { U,indexs, "zero" };
    dd::TDD tdd = tn.to_tdd(dd1.get());
    std::cout << tdd.e.p->v << std::endl;
    dd::export2Dot(tdd.e, "zero");
    return 0;
}

