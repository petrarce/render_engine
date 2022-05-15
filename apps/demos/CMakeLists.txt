find_package(GLM)
return_if(NOT GLM_FOUND)

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

add_executable(demo1
	demo1.cpp)

target_link_libraries(demo1 PUBLIC
	glad
	glfw-context-creator
	opengl-wrapper-core
	geometry-primitives
)
