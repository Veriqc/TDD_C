#include "xtensor/xarray.hpp"
#include "xtensor/xio.hpp"
#include "xtensor/xshape.hpp"
#include <iostream>
#include <xtensor/xadapt.hpp>
xt::xarray<double> arbitrary_cnot(int qubits, int control, int target) {
    auto test = [qubits](int x, int value) {
        return (x / (1 << (qubits - value - 1))) % 2 != 0;
        };

    auto f = [qubits, control, target, test](const int i, const int j) -> double {
        if (!(test(i, control) && test(j, control))) {
            return i == j ? 1.0 : 0.0;
        }
        else if (test(i, target) != test(j, target)) {
            for (int k = 0; k < qubits; ++k) {
                if (k == target || k == control) {
                    continue;
                }
                else if (test(i, k) != test(j, k)) {
                    return 0.0;
                }
            }
            return 1.0;
        }
        else {
            return 0.0;
        }
        };

    int size = 1 << qubits;
    std::vector<double> temp;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            temp.push_back(f(i, j));
            std::cout << "i: " << i << " ";
            std::cout << "j: " << j << " ";
            std::cout << "f: " << f(i,j) << " ";
            std::cout << std::endl;
        }
        std::cout << "-----------------------------" << std::endl;
    }
    xt::xarray<double> result = xt::adapt(temp, { size, size });


    return result;
}

int main() {
    std::cout << arbitrary_cnot(2, 0, 1);
}