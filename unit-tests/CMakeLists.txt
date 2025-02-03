cmake_minimum_required (VERSION 3.11)
 
project ("test_driver")
 
include(FetchContent)
 
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.0 # or a later release
)
 
FetchContent_MakeAvailable(Catch2)
 
# matrix tests
add_executable(matrix-tests matrix-tests.cpp)
 
target_link_libraries(matrix-tests 
    PRIVATE 
    vector-3d-intf
    Catch2::Catch2WithMain
)

# vector tests
add_executable(vector-tests vector-tests.cpp)
 
target_link_libraries(vector-tests 
    PRIVATE 
    vector-3d-intf
    Catch2::Catch2WithMain
)