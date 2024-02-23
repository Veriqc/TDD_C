#include <xtensor/xarray.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xio.hpp>
#include <dd/GateXarray.hpp>
#include <cmath>
#include <vector>

xt::xarray<dd::ComplexValue> identity(int n) {
    if (n == 1) {
        return xgate::Imat;
    }

    xt::xarray<dd::ComplexValue> I = identity(n - 1);

    // Directly calculate the new shape without intermediate steps
    std::vector<size_t> newShape(2+ I.dimension(), 2); // Fills the new shape with 2s

    xt::xarray<dd::ComplexValue> combined = xt::xarray<dd::ComplexValue>::from_shape(newShape); 
    combined.fill(xgate::complex_zero);
    // Initialized to 0s
    for (size_t i = 0; i < 2; ++i) {
        xt::view(combined, i, i, xt::all(), xt::all()) = I; // Place I in the diagonal blocks
    }
    return combined;
}

xt::xarray<dd::ComplexValue> controlMat(xt::xarray<dd::ComplexValue> mat, int c) {
    if (c == 0) return mat;

    mat = controlMat(mat, c - 1);

    std::vector<size_t> newShape(2 + mat.dimension(), 2); 

    xt::xarray<dd::ComplexValue> combined = xt::xarray<dd::ComplexValue>::from_shape(newShape); 
    combined.fill(xgate::complex_zero);

    xt::view(combined, 0, 0, xt::all(), xt::all()) = identity(mat.dimension() / 2); // Place identity matrix
    xt::view(combined, 1, 1, xt::all(), xt::all()) = mat; // Place mat in the bottom-right block

    return combined;
}
// Example usage
int main() {
    auto I = identity(2);
    std::cout << I << std::endl;
    std::cout << "dim: "<< I.dimension() << " size:" << I.size() << std::endl;

    auto cx = controlMat(xgate::Phasemat(1.5),1);
    std::cout << cx <<std::endl;
    return 0;
}
