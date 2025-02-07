# Quaternion Interface
add_library(quaternion-intf
    INTERFACE
)

target_include_directories(quaternion-intf
    INTERFACE
    .
)

target_link_libraries(quaternion-intf
    INTERFACE
    matrix-intf
)

# Quaternion
add_library(quaternion 
    STATIC
    Quaternion.cpp
)

target_link_libraries(quaternion
    PUBLIC
    quaternion-intf
    PRIVATE
)

set_target_properties(quaternion
    PROPERTIES
    LINKER_LANGUAGE CXX
)