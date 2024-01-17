#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <complex>
#include <string>
#include "dd/Tdd.hpp"
#include "dd/Tensor.hpp"
#include "dd/Cir_tn.hpp"
#include "dd/Tdd.hpp"
#include "dd/Export.hpp"
#include <xtensor/xarray.hpp>
#include <xtensor-python/pyarray.hpp>

namespace py = pybind11;
void BindmNode(py::module& m){
    py::class_<dd::mNode, std::shared_ptr<dd::mNode>>(m, "mNode")
        .def(py::init<>(), "Default constructor for mNode")
        .def_readwrite("e", &dd::mNode::e, "A vector of Edge objects connected to this node")
        .def_readwrite("next", &dd::mNode::next, "Pointer to the next mNode in the structure")
        .def_readwrite("ref", &dd::mNode::ref, "Reference count for this node")
        .def_readwrite("v", &dd::mNode::v, "Variable index (nonterminal) value (-1 for terminal)")
        .def_readwrite("flags", &dd::mNode::flags, "Flags for various states or properties of the node")
        .def_static("isTerminal", &dd::mNode::isTerminal, "Check if a given mNode is the terminal node")
        .def_static("getTerminal", &dd::mNode::getTerminal, "Get the terminal mNode")
        .def("__repr__",
             [](const dd::mNode &node) {
                 return "<mNode v=" + std::to_string(node.v) + ">";
             });
}

void BindmEdge(py::module& m){
    py::class_<dd::Edge<dd::mNode>>(m, "Edge")
        .def(py::init<>()) // Add constructors and member functions as needed
        .def_readwrite("p", &dd::Edge<dd::mNode>::p,"ctype: mNode")
        .def_readwrite("w", &dd::Edge<dd::mNode>::w, "ctype: complex")
        .def("__repr__",
             [](const dd::Edge<dd::mNode> &e) {
                 return "complex number: " 
                 + std::to_string(e.w.r->value) + "+" 
                 + std::to_string(e.w.i->value);
                });
}

void BindTDD(py::module& m){
    py::class_<dd::TDD,std::shared_ptr<dd::TDD>>(m,"tdd")
        .def(py::init<>())
        .def_readwrite("e", &dd::TDD::e,"A mEdge to the root of TDD")
        .def_readwrite("index_set", &dd::TDD::index_set,"A vector of Index objects")
        .def_readwrite("key_2_index",&dd::TDD::key_2_index,"A vector of String objects");
}

void BindPackage(py::module& m){
    py::class_<dd::Package<>,std::shared_ptr<dd::Package<>>>(m, "ddpackage")
        .def(py::init<int>())
        .def_readwrite("order",&dd::Package<>::varOrder)
        .def("__repr__",[](dd::Package<> *ddpack){
            std::string mapAsString;
            for (const auto& pair : ddpack->varOrder) {
                mapAsString += pair.first + ": " + std::to_string(pair.second) + "; ";
            }
            return "ddpack order: "+ mapAsString;
        })
        .def("cont",&dd::Package<>::cont,
            py::arg("tdd1"), py::arg("tdd2"),"cont(tdd1,tdd2)")
        .def_readwrite("test",&dd::Package<>::to_test);
    m.def("init", [](int n) {
            auto pkg = std::make_unique<dd::Package<>>(n);
            return pkg.release();
        },
        py::arg("nqubits"),
    py::return_value_policy::reference);
}

void BindTensor(py::module& m){
    py::class_<dd::Tensor>(m, "Tensor")
        .def(py::init<const xt::xarray<std::complex<double>>&,
                        const std::vector<dd::Index>&,
                        const std::string&>())
        .def("to_tdd", [](dd::Tensor &tensor, dd::Package<> *package, bool show) {
                return tensor.to_tdd(package, show);
            },
            py::arg("ddPackage"),
            py::arg("show")=false,
        py::return_value_policy::reference);
}

void BindTn(py::module& m){
    py::class_<dd::TensorNetwork>(m, "Tn")
        .def(py::init<const std::vector<dd::Tensor>&>())
        .def("infor", &dd::TensorNetwork::infor)
        .def("add",&dd::TensorNetwork::add_ts)
        .def("to_tdd",[](dd::TensorNetwork &tn, dd::Package<> *package) {
                return tn.cont(package);
            },
            py::arg("ddPackage"),
        py::return_value_policy::reference)
        .def("__repr__",
             [](dd::TensorNetwork &tn) {
                 return tn.infor();
             });
}

void BindComplex(py::module& m){
    py::class_<dd::ComplexValue>(m, "ComplexValue","a complex value struct")
        .def(py::init<>())  // Default constructor
        .def(py::init<double, double>())  // Constructor with parameters
        .def_readwrite("r", &dd::ComplexValue::r)  // Expose member r
        .def_readwrite("i", &dd::ComplexValue::i) // Expose member i
        .def("__repr__",[](const dd::ComplexValue& z){
            return "complex number: " 
                 + std::to_string(z.r) + "+" 
                 + std::to_string(z.i);
        });
}

void BindIndex(py::module& m){
    py::class_<dd::Index>(m, "Index")
        .def(py::init<>()) 
        .def(py::init<const std::string, const short>())  
        .def_readwrite("key", &dd::Index::key) 
        .def_readwrite("idx", &dd::Index::idx)
        .def("__repr__",[](const dd::Index& index){
            return "Index key: " 
                 + index.key + " Index idx:" 
                 + std::to_string(index.idx);
        });
}

void Binddraw(py::module& m){
    m.def("draw", [](const dd::mEdge& basic,
                     const std::string& outputFilename,
                     bool colored, bool edgeLabels,
                     bool classic, bool memory,
                     bool show, bool formatAsPolar) {
            try{
                dd::export2Dot(basic, outputFilename, colored, edgeLabels, classic, memory, show, formatAsPolar);
            } catch (const std::exception& e){
                std::cerr << "Exception caught: " << e.what() << std::endl;
                throw;
            }
        }, 
        py::arg("basic"), 
        py::arg("outputFilename"), 
        py::arg("colored") = true, 
        py::arg("edgeLabels") = false, 
        py::arg("classic") = false, 
        py::arg("memory") = false, 
        py::arg("show") = true, 
        py::arg("formatAsPolar") = true
    );
}

void BindCir(py::module& m){
    m.def("qubit_in_cir",&get_qubits_num);
    m.def("cir2tn", [](std::string path, std::string  file_name, dd::Package<> *package) {
                return cir_2_tn(path,file_name,package);
        },py::return_value_policy::reference);
}
PYBIND11_MODULE(TDD, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    BindComplex(m);
    BindIndex(m);
    
    BindmNode(m);
    BindmEdge(m);

    BindPackage(m);
    BindTDD(m);

    BindTensor(m);
    BindTn(m);

    Binddraw(m);
    BindCir(m);
}
