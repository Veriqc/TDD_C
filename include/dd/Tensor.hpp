#pragma once


#include <xtensor/xio.hpp>
#include <xtensor/xview.hpp>
#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include "Tdd.hpp"
#include "Package.hpp"
#include <iostream>
#include <stdexcept>

namespace dd {

    struct Tensor {

        xt::xarray<ComplexValue> data;

        std::vector<Index> index_set;

        std::string name;

        Tensor(const xt::xarray<ComplexValue>& data_ = xt::xarray<ComplexValue>(),
           const std::vector<Index>& index_set_ = std::vector<Index>(),
           const std::string& name_ = "") : data(data_), index_set(index_set_), name(name_) {}

		std::vector<std::string> key(){
			std::vector<std::string> res;
			for (auto& index : this->index_set) {
				res.push_back(index.key);
			}
			return res;
		}
        
		TDD to_tdd(std::unique_ptr<Package<>>& ddpackage) {
			
			if (this->data.dimension() != this->index_set.size()) {
				throw std::runtime_error("action non definies "+ this->name);
			}
			ddpackage->add_map(index_set);

			std::vector<int> order(this->index_set.size());
			std::iota(order.begin(), order.end(), 0);

			std::sort(order.begin(), 
						order.end(), 
						[&](int a, int b) {
							return ddpackage->varOrder[index_set[a].key] < ddpackage->varOrder[index_set[b].key];
							}
					);

			std::cout << "order before xarray_2_edge: " << std::endl;
			for (auto num : order) {
				std::cout << num << " ";
			}
			std::cout << std::endl;
			std::cout << "index set before xarray_2_edge: "<< std::endl;
			for(const auto index: this->index_set){
				std::cout << "index idx " << index.idx ;
				std::cout << " index key " << index.key ;
				std::cout << " index order " << ddpackage->varOrder[index.key] <<std::endl;
			}

			TDD res;
			res.e = ddpackage->xarray_2_edge(data, order);
			std::cout << "finish xarray_2_edge" << std::endl;

			std::vector<Index> index_set = this->index_set;
			std::sort(index_set.begin(),
				index_set.end(),
				[&](const auto& a, const auto& b) {return ddpackage->varOrder[a.key] < ddpackage->varOrder[b.key]; }
			);
			std::cout << "index set after xarray_2_edge: "<< std::endl;
			for(const auto index: this->index_set){
				std::cout << "index idx " << index.idx ;
				std::cout << " index key " << index.key ;
				std::cout << " index order " << ddpackage->varOrder[index.key] <<std::endl;
			}
			std::cout << "index set initial: "<< std::endl;
			for(const auto index: index_set){
				std::cout << "index idx " << index.idx ;
				std::cout << " index key " << index.key ;
				std::cout << " index order " << ddpackage->varOrder[index.key] <<std::endl;
			}
			std::cout << "finish index_set initial" << std::endl;


			res.key_2_index = this->key();
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
            std::cout << "et j'ai " << this->tensors.size() << " tenseur" << std::endl;
        }

        TDD cont(std::unique_ptr<Package<>>& ddpackage) {
			if (this->tensors.size() == 0) {
				throw std::runtime_error("null tensor network");
			}
			TDD dd_temp = tensors[0].to_tdd(ddpackage);
			for (int i = 1; i < this->tensors.size(); ++i) {
				std::cout << "-------------------------" <<std::endl;
				std::cout << i+1 << "th" <<"/" << this->tensors.size() << " tdd:" << std::endl;
				dd_temp = ddpackage->cont(dd_temp, tensors[i].to_tdd(ddpackage));
			}
			return dd_temp;
        };
    };


}