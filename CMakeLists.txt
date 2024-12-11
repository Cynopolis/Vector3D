cmake_minimum_required(VERSION 3.6)
project(Vector3D)

add_subdirectory(unit-tests)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-fdiagnostics-color=always)

# Vector3d
add_library(Vector3D 
    STATIC
    Vector3D.hpp
)

set_target_properties(Vector3D
    PROPERTIES
    LINKER_LANGUAGE CXX
)

target_include_directories(Vector3D PUBLIC
    include
)

# Matrix
add_library(Matrix 
    STATIC
    Matrix.hpp
)

set_target_properties(Matrix
    PROPERTIES
    LINKER_LANGUAGE CXX
)

target_include_directories(Matrix 
    PUBLIC
    .
)