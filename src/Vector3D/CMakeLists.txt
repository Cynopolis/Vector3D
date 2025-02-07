# Vector 3D Interface
add_library(vector-3d-intf
    INTERFACE
)

target_include_directories(vector-3d-intf
    INTERFACE
    .
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