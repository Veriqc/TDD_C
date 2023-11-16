#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "dd/Tdd.hpp"
#include "dd/Tensor.hpp"
#include "Cir_tn.hpp"
#include <xtensor/xarray.hpp>
#include <xtensor-python/pyarray.hpp>
class xarrayClass {
    private:
    xt::xarray<double> data;
    std::vector<xt::xarray<double>> datas;
    public:
        xarrayClass(const xt::xarray<double>& data_,const std::vector<xt::xarray<double>>& datas_ = {}):
        data(data_), datas(datas_){}

        void add(xt::xarray<double> array){
            this->datas.push_back(array);
        }
        void infor(){
            std::cout << "data: " << this->data << std::endl;
            std::cout << "datas: "<< std::endl;
            for(auto array: this->datas){
                std::cout << array << std::endl;
            }
            
        }
};


int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;

PYBIND11_MODULE(TDD, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");

    // m.def("Cir_2_tn", &cir_2_tn, "A function which convert qasm to tensornetwork");
    py::class_<xarrayClass>(m, "xarrayClass")
        .def(py::init<const xt::xarray<double>&, const std::vector<xt::xarray<double>>&>())
        .def("add", &xarrayClass::add)
        .def("infor", &xarrayClass::infor);



    // py::class_<dd::TensorNetwork>(m, "Tensor")
    // .def(py::init<>())
    // .def("add",&Vector2D::add)
    // .def("re",&Vector2D::reverse)
    // .def("infor",&Vector2D::info);

}
