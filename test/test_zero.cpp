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

    auto e = dd1->makeZeroState(4);
    dd::export2Dot(e, "zero");
    return 0;
}

