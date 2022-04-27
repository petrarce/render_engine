# static library
include_directories(AFTER .)
include_directories(AFTER ../core/)

add_library(physics STATIC 
                ./RigidBody.cpp
                ./RigidBodySystem.cpp)
add_dependencies(physics core)
target_link_libraries(physics core)
# install libs
install(TARGETS physics DESTINATION lib/)
