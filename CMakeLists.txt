cmake_minimum_required(VERSION 3.16)

project(space_sim_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

pybind11_add_module(
    space_sim_cpp

    bindings/bindings.cpp

    core/body.cpp
    core/vector3d.cpp
    core/gravity.cpp
    core/integrator.cpp
    core/octnode.cpp
    core/gw_decay.cpp

    pulsars/pulsars.cpp
)

target_include_directories(
    space_sim_cpp
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/core
        ${CMAKE_CURRENT_SOURCE_DIR}/pulsars
)