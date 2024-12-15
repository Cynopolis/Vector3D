cmake_minimum_required (VERSION 3.11)
 
project ("test_driver")
 
include(FetchContent)
 
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.1 # or a later release
)
 
FetchContent_MakeAvailable(Catch2)
 
add_executable(matrix-tests matrix-tests.cpp)
 
target_link_libraries(matrix-tests 
    PRIVATE 
    Matrix
    Catch2::Catch2WithMain
)