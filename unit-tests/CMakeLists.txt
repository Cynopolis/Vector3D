# Quaternion tests
add_executable(quaternion-tests quaternion-tests.cpp)
 
target_link_libraries(quaternion-tests 
    PRIVATE 
    quaternion
    Catch2::Catch2WithMain
)

# matrix tests
add_executable(matrix-tests matrix-tests.cpp)
 
target_link_libraries(matrix-tests 
    PRIVATE 
    matrix
    Catch2::Catch2WithMain
)

# Vector 3D Tests
add_executable(vector-3d-tests vector-tests.cpp)

target_link_libraries(vector-3d-tests
    PRIVATE
    vector-3d
    Catch2::Catch2WithMain
)