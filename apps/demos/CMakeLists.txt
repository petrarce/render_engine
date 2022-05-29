find_package(GLM)
return_if(NOT GLM_FOUND)

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
set (LIBS
	glad
	glfw-context-creator
	opengl-wrapper-core
	geometry-primitives
	gl-components
)
add_executable(demo1
	demo1.cpp)

target_link_libraries(demo1 PUBLIC
	${LIBS}
)


add_executable(demo2
	demo2.cpp)

target_link_libraries(demo2 PUBLIC
	${LIBS}
)
