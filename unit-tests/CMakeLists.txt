# Quaternion tests
add_executable(quaternion-tests unit-tests/quaternion-tests.cpp)
 
target_link_libraries(quaternion-tests 
    PRIVATE 
    vector-3d-intf
    Catch2::Catch2WithMain
)

# matrix tests
add_executable(matrix-tests unit-tests/matrix-tests.cpp)
 
target_link_libraries(matrix-tests 
    PRIVATE 
    vector-3d-intf
    Catch2::Catch2WithMain
)

# Vector 3D Tests
add_executable(vector-3d-tests unit-tests/vector-tests.cpp)

target_link_libraries(vector-3d-tests
    PRIVATE
    vector-3d-intf
    Catch2::Catch2WithMain
)