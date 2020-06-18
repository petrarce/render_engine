include_directories(AFTER ../core/)
include_directories(AFTER ../physics/)
set(LINK_LIBRARIES core GL GLEW SDL2 SDL2_image assimp)

add_executable(ModelLoading skeletal/main.cpp)
add_dependencies(ModelLoading core physics)
target_link_libraries(ModelLoading ${LINK_LIBRARIES} physics)

add_executable(simple simple/main.cpp)
add_dependencies(simple core)
target_link_libraries(simple ${LINK_LIBRARIES})

add_executable(example-shadergenerator example_shadergenerator/main.cpp)
add_dependencies(example-shadergenerator molecular-programgenerator molecular-util)
target_link_libraries(example-shadergenerator molecular-programgenerator molecular-util)
