#include <Python.h>
#include <string>

struct Index {
        std::string key; //
        short idx;
        Index(const std::string key_, const short idx_) :
            key(key_), idx(idx_) {}
};
static PyTypeObject Index_py = {
    PyVarObject_HEAD_INIT(NULL, 0)
    .tp_name = "Index",  // Fully-qualified name of the Python class
    .tp_basicsize = sizeof(Index),    // Size of your C data structure
    .tp_itemsize = 0,
    .tp_dealloc = NULL,                   // Use a deallocator function if needed
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_doc = "Index data type",
};

static Index convert_to_c(PyObject* py_object) {
    if (PyObject_TypeCheck(py_object, &Index_py)) {
        Index index(
            PyLong_AsLong(PyObject_GetAttrString(py_object, "key")),
            PyLong_AsLong(PyObject_GetAttrString(py_object, "idx")));
        return index;
    }

    PyErr_SetString(PyExc_TypeError, "Object is not of the expected type");
    Index index = {" ", 0};
    return index;
}

static PyObject* my_module_my_function(PyObject* self, PyObject* args) {
    Index index = convert_to_c(args);

    // Use c_data in your C code
    return Py_BuildValue("i", index.idx);
}

static PyMethodDef my_module_methods[] = {
    {"my_function", my_module_my_function, METH_VARARGS, "Documentation for my_function"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef my_module = {
    PyModuleDef_HEAD_INIT,
    "tdd",        // Module name
    "Documentation for tdd",
    -1,
    my_module_methods
};

PyMODINIT_FUNC PyInit_tdd(void) {
    return PyModule_Create(&my_module);
}
