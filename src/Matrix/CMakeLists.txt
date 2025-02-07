# Matrix Interface
add_library(matrix-intf
    INTERFACE
)

target_include_directories(matrix-intf
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
    matrix-intf
    PRIVATE
)

set_target_properties(matrix
    PROPERTIES
    LINKER_LANGUAGE CXX
)

# matrix tests
add_executable(matrix-tests unit-tests/matrix-tests.cpp)
 
target_link_libraries(matrix-tests 
    PRIVATE 
    matrix-intf
    Catch2::Catch2WithMain
)