#pragma once


#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include "Tdd.hpp"
#include "Package.hpp"
#include <iostream>

namespace dd {

    struct Tensor {

        xt::xarray<ComplexValue> data;

        std::vector<Index> index_set;

        std::string name;

        Tensor(const xt::xarray<ComplexValue>& data_ = xt::xarray<ComplexValue>(),
           const std::vector<Index>& index_set_ = std::vector<Index>(),
           const std::string& name_ = "") : data(data_), index_set(index_set_), name(name_) {}
        
		TDD to_tdd(std::unique_ptr<Package<>>& ddpackage) {
			if (data.dimension() != index_set.size()) {
				throw "action non definies";
			}
			ddpackage->add_map(index_set);

			std::vector<int> order(index_set.size());
			std::iota(order.begin(), order.end(), 0);

			std::sort(order.begin(), order.end(), [&](int a, int b) {
				return ddpackage->varOrder[index_set[a].key] < ddpackage->varOrder[index_set[b].key];
				});

			for (auto num : order) {
				std::cout << num << " ";
			}
			std::cout << std::endl;

			TDD res;
			res.e = ddpackage->xarray_2_edge(data, order);

			std::vector<Index> index_set = index_set;
			std::sort(index_set.begin(),
				index_set.end(),
				[&](const auto& a, const auto& b) {return ddpackage->varOrder[a.key] < ddpackage->varOrder[b.key]; }
			);


			res.key_2_index = ddpackage->generate_key(index_set);
			res.index_set = index_set;

			std::cout << "TDD index set:" << std::endl;
			for (auto index : res.index_set) {
				std::cout << "index key: " << index.key << "	index idx: " << index.idx << std::endl;
			}
			std::cout << "TDD index:" << std::endl;
			for (auto key : res.key_2_index) {
				std::cout << "key: " << key << std::endl;
			}
			return res;
        }
    };

    struct TensorNetwork {
        std::vector<Tensor> tensors;
        void infor() {
            std::cout << "Je suis un TensorNetwork." << std::endl;
            std::cout << "et j'ai " << tensors.size() << " tenseur" << std::endl;
        }

        TDD cont(std::unique_ptr<Package<>>& ddpackage) {
			TDD dd_temp = tensors[0].to_tdd(ddpackage);
			for (int i = 1; i < tensors.size(); ++i) {
				dd_temp = ddpackage->cont(dd_temp, tensors[i].to_tdd(ddpackage));
			}
			return dd_temp;
        };
    };


}