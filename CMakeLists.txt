cmake_minimum_required (VERSION 3.11)
project(Vector3D)

add_subdirectory(src)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(-fdiagnostics-color=always -Wall -Wextra -Wpedantic)

include(FetchContent)
 
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.0 # or a later release
)
 
FetchContent_MakeAvailable(Catch2)