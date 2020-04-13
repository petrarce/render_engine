# compile main executable
add_executable(ModelLoading main.cpp)
add_dependencies(ModelLoading core physics)
# link library
target_link_libraries(ModelLoading core physics GL GLEW SDL2 SDL2_image assimp)
