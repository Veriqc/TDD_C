#include <xtensor/xarray.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xio.hpp>
#include <cmath>
#include <vector>

// Function to simulate splitting and reshaping
xt::xarray<int> identity(int n) {
    if (n==1){
        return {{1, 0},{0, 1}};
    }

    xt::xarray<int> I = identity(n-1);

    auto originalShape = I.shape();
    xt::xarray<int> zero = xt::xarray<int>::from_shape(originalShape);
    zero.fill(0);
    std::vector<size_t> newShape(originalShape.begin(), originalShape.end());
    newShape.insert(newShape.begin(), {2, 2});

    xt::xarray<int> combined = xt::xarray<int>::from_shape(newShape);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (i==j)
            xt::view(combined, i, j) = I;
            else
            xt::view(combined, i, j) = zero;
        }
    }
    return combined;
}

xt::xarray<int> controlMat(xt::xarray<int> mat, int c){
    if(c==0) return mat;

    mat = controlMat(mat,c-1);
    auto originalShape = mat.shape();
    int n = mat.dimension()/2;
    xt::xarray<int> zero = xt::xarray<int>::from_shape(originalShape);
    zero.fill(0);
    std::vector<size_t> newShape(originalShape.begin(), originalShape.end());
    newShape.insert(newShape.begin(), {2, 2});

    xt::xarray<int> combined = xt::xarray<int>::from_shape(newShape);
    xt::view(combined, 0, 0) = identity(n);
    xt::view(combined, 0, 1) = zero;
    xt::view(combined, 1, 0) = zero;
    xt::view(combined, 1, 1) = mat;
    return combined;
}

// Example usage
int main() {
    auto I = identity(3);
    std::cout << I << std::endl;
    std::cout << I.dimension() << std::endl;

    auto cx = controlMat({{0, 1},{1, 0}},1);
    std::cout << cx <<std::endl;
    return 0;
}
