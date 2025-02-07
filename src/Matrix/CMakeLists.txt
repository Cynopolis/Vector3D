# Vector 3D Interface
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
    vector-3d-intf
    PRIVATE
)

set_target_properties(matrix
    PROPERTIES
    LINKER_LANGUAGE CXX
)