#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/operators.h>
#include "../core/vector3d.h"
#include "../core/body.h"
#include "../core/integrator.h"
#include "../core/gw_decay.h"
#include "../pulsars/pulsars.h"

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<Body>);

PYBIND11_MODULE(space_sim_cpp, m) {
    py::class_<vector3d>(m, "Vector3D")
        .def(py::init<double, double, double>())
        .def_readwrite("x", &vector3d::x)
        .def_readwrite("y", &vector3d::y)
        .def_readwrite("z", &vector3d::z)
        .def("add", &vector3d::add)
        .def("sub", &vector3d::sub)
        .def("mult", &vector3d::mult)
        .def("magnitude", &vector3d::magnitude)
        .def("normalized", &vector3d::normalized)
        .def("dot", &vector3d::dot)
        .def("cross", &vector3d::cross)
        .def(py::self + py::self)
        .def(py::self - py::self);

    py::class_<Body>(m, "Body")
        .def(py::init<std::string, double, vector3d, vector3d, double>())
        .def_readwrite("name", &Body::name)
        .def_readwrite("mass", &Body::mass)
        .def_readwrite("position", &Body::position)
        .def_readwrite("velocity", &Body::velocity)
        .def_readwrite("radius", &Body::radius)
        .def_readwrite("binary_id", &Body::binary_id)
        .def_readwrite("stellar_type", &Body::stellar_type)
        .def_readwrite("active", &Body::active)
        .def_readwrite("just_merged", &Body::just_merged);

    py::bind_vector<std::vector<Body>>(m, "BodyVector");

    m.def("leapfrog_step", &leapfrog_step,
          py::arg("bodies"), py::arg("dt"), py::arg("is_first_step") = false,
          "Advance the simulation by one Leapfrog step");

    py::enum_<StellarType>(m, "StellarType")
        .value("PROTOSTAR", StellarType::PROTOSTAR)
        .value("RED_GIANT", StellarType::RED_GIANT)
        .value("WHITE_DWARF", StellarType::WHITE_DWARF)
        .value("NEUTRON_STAR", StellarType::NEUTRON_STAR)
        .value("BLACK_HOLE", StellarType::BLACK_HOLE)
        .value("SUPERNOVA", StellarType::SUPERNOVA)
        .value("NONE", StellarType::NONE);

    py::class_<binarystate>(m, "BinaryState")
        .def_readwrite("binary_id", &binarystate::binary_id)
        .def_readwrite("a", &binarystate::a)
        .def_readwrite("e", &binarystate::e)
        .def_readwrite("nu", &binarystate::nu)
        .def_readwrite("m1", &binarystate::m1)
        .def_readwrite("m2", &binarystate::m2)
        .def_readwrite("idx1", &binarystate::idx1)
        .def_readwrite("idx2", &binarystate::idx2);

    m.def("create_binary_state", &create_binary_state,
    py::arg("body1"), py::arg("body2"), py::arg("binary_id"), py::arg("idx1"), py::arg("idx2"), py::arg("m1"), py::arg("m2")
    ,"Creates the binary state for the bodies"
    );

    m.def("decay_orbital_elements", &decay_orbital_elements,
    py::arg("state"), py::arg("m1"), py::arg("m2"), py::arg("dt"), py::arg("speedup"),
    "Applies Peters (1964) GW decay to shrink a and e"
    );

    m.def("advance_true_anomaly", &advance_true_anomaly,
        py::arg("state"), py::arg("m1"), py::arg("m2"), py::arg("dt"),
        "Advances true anomaly nu forward in time"
    );

    m.def("apply_orbital_state", &apply_orbital_state,
        py::arg("body1"), py::arg("body2"), py::arg("state"),
        "Writes updated position/velocity back onto both bodies from orbital state"
    );

    m.def("update_binaries", &update_binaries,
      py::arg("bodies"), py::arg("dt"),
      "Updates all active binary pairs via GW decay and writes results back into bodies");
    
    py::class_<PulsarConfig>(m, "PulsarConfig")
        .def(py::init<double, double, double, double, vector3d>())
        .def_readwrite("stellar_radius", &PulsarConfig::stellar_radius)
        .def_readwrite("speed_of_light", &PulsarConfig::speed_of_light)
        .def_readwrite("polar_field_strength", &PulsarConfig::polar_field_strength)
        .def_readwrite("omega", &PulsarConfig::omega)
        .def_readwrite("magnetic_axis", &PulsarConfig::magnetic_axis);

    m.def("rotation_period", &rotation_period,
    py::arg("omega"), "Returns the time required for one full rotation from the angular velocity"
    );

    m.def("light_cylinder_radius", &light_cylinder_radius,
    py::arg("speed_of_light"),
    py::arg("omega")
    );

    m.def("get_magnetic_field", &get_magnetic_field,
    py::arg("config"),
    py::arg("position")
    );

}