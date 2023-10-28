#pragma once


#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include "Tdd.hpp"

namespace dd {

    struct Tensor {

        xt::xarray<ComplexValue> data;

        std::vector<Index> index_set;

        std::string name;

        Tensor(const xt::xarray<ComplexValue>& data_ = xt::xarray<ComplexValue>(),
           const std::vector<Index>& index_set_ = std::vector<Index>(),
           const std::string& name_ = "") : data(data_), index_set(index_set_), name(name_) {}
    };


    struct TensorNetwork {
        std::vector<Tensor> tensors;
        void infor() {
            std::cout << "Je suis un TensorNetwork." << std::endl;
            std::cout << "et j'ai " << tensors.size() << " tenseur" << std::endl;
        }

        dd::TDD cont();
    };


}