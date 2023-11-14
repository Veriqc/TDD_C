#include <pybind11/pybind11.h>
#include "Cir_tn.hpp"

class Vector2D {
public:
    Vector2D(double x, double y) : x(x), y(y) {}

    double x, y; // For simplicity, let's make these public

    double add(){
        return this->x+this->y ;
    }

    Vector2D reverse(){
        return Vector2D(this->y, this->x);
    }

    void info(){
        std::cout<< "x:" << x << " y:" << y  << std::endl;
    }
};

Vector2D add_vectors(const Vector2D& v1, const Vector2D& v2) {
    return Vector2D(v1.x + v2.x, v1.y + v2.y);
}

int add(int i, int j) {
    return i + j;
}

namespace py = pybind11;

PYBIND11_MODULE(TDD, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function which adds two numbers");

    // m.def("Cir_2_tn", &cir_2_tn, "A function which convert qasm to tensornetwork");
    py::class_<Vector2D>(m, "Vector2D")
    .def(py::init<double, double>())
    .def_readwrite("x", &Vector2D::x)
    .def_readwrite("y", &Vector2D::y)
    .def("add",&Vector2D::add)
    .def("re",&Vector2D::reverse)
    .def("infor",&Vector2D::info);

    m.def("add_vectors", &add_vectors);

}
