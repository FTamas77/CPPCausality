#include <pybind11/pybind11.h>
#include <Windows.h>

#include "engine_api.h"

namespace py = pybind11;

PYBIND11_MODULE(engine_api, m) {
    m.def("add_numbers", &add);
}

