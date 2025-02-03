cmake_minimum_required(VERSION 3.6)
project(Vector3D)

add_subdirectory(unit-tests)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-fdiagnostics-color=always)

# Vector 3D Interface
add_library(vector-3d-intf
    INTERFACE
)

target_include_directories(vector-3d-intf
    INTERFACE
    .
)

# Matrix
add_library(matrix 
    STATIC
    Matrix.cpp
)

target_link_libraries(matrix
    PUBLIC
    vector-3d-intf
    PRIVATE
)

set_target_properties(matrix
    PROPERTIES
    LINKER_LANGUAGE CXX
)

# Vector3d
add_library(vector-3d 
    STATIC
    Vector3D.hpp
)

target_include_directories(vector-3d 
    PUBLIC
    .
)

target_link_libraries(vector-3d
    PUBLIC
    vector-3d-intf
    PRIVATE
)

set_target_properties(vector-3d
    PROPERTIES
    LINKER_LANGUAGE CXX
)