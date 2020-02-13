# compile main executable
include_directories(AFTER ${CMAKE_HOME_DIRECTORY}/src/core/include/)
add_executable(ModelLoading main.cpp)
add_dependencies(ModelLoading core)
# link library
target_link_libraries(ModelLoading core GL GLEW SDL2 SDL2_image assimp)
