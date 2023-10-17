#include <iostream>
#include <xtensor/xarray.hpp>
#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include <dd/Package.hpp>

int main(int argc, char* argv[])
{
    xt::xarray<double> arr
      {{1.0, 2.0, 3.0},
       {2.0, 5.0, 7.0},
       {2.0, 5.0, 7.0}};


    // dd::array_2_edge(arr,5);

    return 0;
}