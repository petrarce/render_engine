# compile main executable
add_executable(ModelLoading main.cpp)
add_dependencies(ModelLoading core imgui)
# link library
target_link_libraries(ModelLoading imgui core GL GLEW SDL2 SDL2_image assimp)
