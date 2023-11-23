#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <complex>
#include "dd/Tdd.hpp"
#include "dd/Tensor.hpp"
#include "Cir_tn.hpp"
#include "dd/Tdd.hpp"
#include <xtensor/xarray.hpp>
#include <xtensor-python/pyarray.hpp>

class xarrayClass {
    private:
    xt::xarray<dd::ComplexValue> data;
    std::vector<xt::xarray<dd::ComplexValue>> datas;

    public:
        xarrayClass(const xt::xarray<std::complex<double>>& data_,const std::vector<xt::xarray<std::complex<double>>>& datas_ = {}): data(xarray_convert(data_)), datas() {
            for(auto array: datas_){
                this->datas.push_back(xarray_convert(array));
            }
        }

        xarrayClass(const xt::xarray<dd::ComplexValue>& data_,const  std::vector<xt::xarray<dd::ComplexValue>>& datas_ = {}): data(data_), datas(datas_) {}

        void add(xt::xarray<std::complex<double>> array){
            this->datas.push_back(xarray_convert(array));
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

class PackageWrapper {
public:
    PackageWrapper(int n) : pkg(std::make_unique<dd::Package<>>(n)) {}

    // Any other necessary methods

private:
    std::unique_ptr<dd::Package<>> pkg;
};


namespace py = pybind11;
void BindArray(py::module& m){
    py::class_<xarrayClass>(m, "xarrayClass")
        .def(py::init<const xt::xarray<std::complex<double>>&, const std::vector<xt::xarray<std::complex<double>>>&>())
        .def("add", &xarrayClass::add)
        .def("infor", &xarrayClass::infor);

}

void BindmNode(py::module& m){
    py::class_<dd::mNode>(m, "mNode")
        .def(py::init<>()) // Default constructor
        .def_readwrite("e", &dd::mNode::e)
        .def_readwrite("next", &dd::mNode::next)
        .def_readwrite("ref", &dd::mNode::ref)
        .def_readwrite("v", &dd::mNode::v)
        .def_readwrite("flags", &dd::mNode::flags)
        .def_static("isTerminal", &dd::mNode::isTerminal)
        .def_static("getTerminal", &dd::mNode::getTerminal);
}

void BindmEdge(py::module& m){
    py::class_<dd::Edge<dd::mNode>>(m, "mEdge")
        .def(py::init<>()) // Default constructor
        .def_readwrite("p", &dd::Edge<dd::mNode>::p)
        .def_readwrite("w", &dd::Edge<dd::mNode>::w);
}

void BindTDD(py::module& m){
    py::class_<dd::TDD>(m,"tdd")
        .def(py::init<>())
        .def_readwrite("e", &dd::TDD::e)
        .def_readwrite("index_set", &dd::TDD::index_set)
        .def_readwrite("key_2_index",&dd::TDD::key_2_index);
}

void BindPackage(py::module& m){
    py::class_<dd::Package<>>(m, "ddpackage")
        .def(py::init<int>())
        .def_readwrite("order",&dd::Package<>::varOrder);
    m.def("ddtable", [](int n) {
        auto pkg = std::make_unique<dd::Package<>>(n);
        return pkg.release();
    }, py::return_value_policy::take_ownership);
}

void BindTensor(py::module& m){
    py::class_<dd::Tensor>(m, "Tensor")
        .def(py::init<const xt::xarray<std::complex<double>>&,
                        const std::vector<dd::Index>&,
                        const std::string&>())
        .def("to_tdd", [](dd::Tensor &tensor, dd::Package<> *package) {
        std::unique_ptr<dd::Package<>> ptr(package);
        tensor.to_tdd(ptr);
        });
}

void BindTn(py::module& m){
    py::class_<dd::TensorNetwork>(m, "Tn")
        .def(py::init<const std::vector<dd::Tensor>&>())
        .def("infor", &dd::TensorNetwork::infor)
        .def("add",&dd::TensorNetwork::add_ts);
}

void BindComplex(py::module& m){
    py::class_<dd::ComplexValue>(m, "ComplexValue")
        .def(py::init<>())  // Default constructor
        .def(py::init<double, double>())  // Constructor with parameters
        .def_readwrite("r", &dd::ComplexValue::r)  // Expose member r
        .def_readwrite("i", &dd::ComplexValue::i); // Expose member i
}

void BindIndex(py::module& m){
    py::class_<dd::Index>(m, "Index")
        .def(py::init<>()) 
        .def(py::init<const std::string, const short>())  
        .def_readwrite("key", &dd::Index::key) 
        .def_readwrite("idx", &dd::Index::idx); 
}
PYBIND11_MODULE(TDD, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");

    
    BindComplex(m);
    BindIndex(m);
    BindArray(m);
    

    BindmNode(m);
    BindmEdge(m);

    BindPackage(m);
    BindTDD(m);

    BindTensor(m);
    BindTn(m);
}
