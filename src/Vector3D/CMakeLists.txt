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

# vector tests
add_executable(vector-tests unit-tests/vector-tests.cpp)
 
target_link_libraries(vector-tests 
    PRIVATE 
    matrix-intf
    vector-3d-intf
    Catch2::Catch2WithMain
)