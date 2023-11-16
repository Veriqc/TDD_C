#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include "Tdd.hpp"
#include "Package.hpp"
#include <iostream>
#include <stdexcept>

namespace dd {
	std::vector<int> reOrder(const std::vector<int>& index_set) {
		std::vector<int> sorted_index_set = index_set;
		std::sort(sorted_index_set.begin(), sorted_index_set.end());

		std::map<int, int> index_map;
		int rank = 0;

		for (int i = 0; i < sorted_index_set.size(); ++i) {
			if (i == 0 || sorted_index_set[i] != sorted_index_set[i - 1]) {
				index_map[sorted_index_set[i]] = rank++;
			}
		}

		std::vector<int> order;
		for (int value : index_set) {
			order.push_back(index_map[value]);
		}

		return order;
	}

    class Tensor {
		private:
			xt::xarray<ComplexValue> data;

			std::vector<Index> index_set;

			std::string name;
		public:
			Tensor(const xt::xarray<ComplexValue>& data_ = xt::xarray<ComplexValue>(),
			const std::vector<Index>& index_set_ = std::vector<Index>(),
			const std::string& name_ = "") : data(data_), index_set(index_set_), name(name_) {}
			
			TDD to_tdd(std::unique_ptr<Package<>>& ddpackage) {
				
				if (this->data.dimension() != this->index_set.size()) {
					std::cerr << "dim: " << this->data.dimension() << std::endl;
					std::cerr << "index: " << this->index_set.size() << std::endl;
					throw std::runtime_error("action non definies "+ this->name);
				}

				std::vector<int> order;
				for(const auto index: this->index_set){
					auto it = ddpackage->varOrder.find(index.key);
					if (it == ddpackage->varOrder.end()) {
						int num = ddpackage->update_map_value();
						ddpackage->varOrder[index.key] = num;
					}
					order.push_back(ddpackage->varOrder[index.key]);
				}
				order = reOrder(order);

				TDD res;
				res.e = ddpackage->xarray_2_edge(data, order);
				
				std::vector<Index> temp_index_set = this->index_set;
				std::sort(temp_index_set.begin(),
					temp_index_set.end(),
					[&](const auto& a, const auto& b) {return ddpackage->varOrder[a.key] < ddpackage->varOrder[b.key]; }
				);
			
				res.key_2_index = {};
				for(const auto index: temp_index_set){
					res.key_2_index.push_back(index.key);
				}

				res.index_set = temp_index_set;

				return res;
			}
    };

    class TensorNetwork {
		private:
			std::vector<Tensor> tensors;
		public:
			void add_ts(Tensor ts){
				this->tensors.push_back(ts);
			}
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
				try{
					dd_temp = ddpackage->cont(dd_temp, tensors[i].to_tdd(ddpackage));
				}
				catch(...){
					std::exception_ptr p = std::current_exception();
					std::clog <<(p ? p.__cxa_exception_type()->name() : "null") << std::endl;
				}
			}
			return dd_temp;
        };
    };


}
#endif