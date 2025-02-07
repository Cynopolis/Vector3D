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