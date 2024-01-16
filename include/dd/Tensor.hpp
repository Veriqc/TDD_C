#ifndef TENSOR_HPP
#define TENSOR_HPP

#include <xtensor/xarray.hpp>
#include <xtensor/xtensor.hpp>
#include <complex>
#include "Tdd.hpp"
#include "Package.hpp"
#include "Export.hpp"
#include <iostream>
#include <stdexcept>

// TODO: hide the following two functions
dd::ComplexValue Complex_Convert(const std::complex<double>& num){
    return dd::ComplexValue(num.real(),num.imag());
}

xt::xarray<dd::ComplexValue> xarray_convert(const xt::xarray<std::complex<double>>& data){
    xt::xarray<dd::ComplexValue> convertedArray(data.shape());
    int index = 0;
    for(auto& num: data){
        convertedArray[index++] = Complex_Convert(num);
    }
    return convertedArray;
}
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
namespace dd {
    class Tensor {
		private:
			xt::xarray<ComplexValue> data;

			std::vector<Index> index_set;

			std::string name;
		public:
			Tensor(const xt::xarray<ComplexValue>& data_ ,
			const std::vector<Index>& index_set_ = std::vector<Index>(),
			const std::string& name_ = "") : data(data_), index_set(index_set_), name(name_) {}

			Tensor(const xt::xarray<std::complex<double>>& data_ ,
			const std::vector<Index>& index_set_ = std::vector<Index>(),
			const std::string& name_ = "") : data(xarray_convert(data_)), index_set(index_set_), name(name_) {}
			TDD to_tdd(Package<>* ddpackage, const bool show = false) {
				if (!ddpackage) {
					throw std::runtime_error("ddpackage is null");
				}
				
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
					if (res.key_2_index.empty() || res.key_2_index.back() != index.key) {
						res.key_2_index.push_back(index.key);
					}
				}

				res.index_set = temp_index_set;
				if(show){
					export2Dot(res.e,this->name);
				}
				return res;
			}
    };

    class TensorNetwork {
		public:
			std::vector<Tensor> tensors;
		public:
			TensorNetwork(std::vector<Tensor> tensors_={}): tensors(tensors_) {}
			void add_ts(Tensor ts){
				this->tensors.push_back(ts);
			}
			std::string infor() {
				return "a tensor network with " + std::to_string(this->tensors.size()) + " tensors";
			}

			TDD cont(Package<>* ddpackage) {
				if (!ddpackage) {
					throw std::runtime_error("ddpackage is null");
				}
				if (this->tensors.size() == 0) {
					throw std::runtime_error("null tensor network");
				}
				TDD dd_temp = tensors[0].to_tdd(ddpackage);
				for (int i = 1; i < this->tensors.size(); ++i) {
					// std::cout << "-------------------------" <<std::endl;
					// std::cout << i+1 << "th" <<"/" << this->tensors.size() << " tdd:" << std::endl;
					try{
						dd_temp = ddpackage->cont(dd_temp, this->tensors[i].to_tdd(ddpackage));
						// dd_temp = this->tensors[i].to_tdd(ddpackage);
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