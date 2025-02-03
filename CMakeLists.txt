cmake_minimum_required(VERSION 3.6)
project(Vector3D)

add_subdirectory(unit-tests)
add_subdirectory(src)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-fdiagnostics-color=always)