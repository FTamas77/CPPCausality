
#include <pybind11/pybind11.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "engine_api.h"

namespace py = pybind11;

PYBIND11_MODULE(engine_api, m) {
    m.def("add_numbers", &add);
}

