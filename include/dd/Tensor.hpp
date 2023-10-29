#pragma once


#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include "Tdd.hpp"
#include "Package.hpp"

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

        /*TDD cont(std::unique_ptr<dd::Package<>> ddpackage) {
            std::vector<TDD> tdds;
            for (auto ts : tensors) {
                tdds.push_back(ddpackage->Tensor_2_TDD(ts));
            }
            TDD dd_temp = tdds[0];
            for (int i = 1; i < tdds.size(); ++i) {
                dd_temp = ddpackage->cont(dd_temp, tdds[i]);
            }
            return dd_temp;
        };*/
    };


}