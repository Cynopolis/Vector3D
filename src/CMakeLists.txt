# Quaternion Interface
add_library(vector-3d-intf
    INTERFACE
)

target_include_directories(vector-3d-intf
    INTERFACE
    .
)

target_link_libraries(vector-3d-intf
    INTERFACE
)

# Quaternion
add_library(quaternion 
    STATIC
    Quaternion.cpp
)

target_link_libraries(quaternion
    PUBLIC
    vector-3d-intf
    PRIVATE
)

set_target_properties(quaternion
    PROPERTIES
    LINKER_LANGUAGE CXX
)

# Vector3d
add_library(vector-3d 
    STATIC
    Vector3D.cpp
)

target_link_libraries(vector-3d
    PUBLIC
    vector-3d-intf
    PRIVATE
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