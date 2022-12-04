find_package(X11)
return_if(NOT X11_FOUND)
include_directories(.)
find_package(GLFW3)
if(GLFW3_FOUND)
	add_library(glfw-context-creator STATIC
		OpenGLGlfwContextCreator.cpp
		OpenGLGlfwContextCreator.h
		GLGlfwContextCreator.cpp
		GLGlfwContextCreator.hpp
	)
	set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")

	target_link_libraries(glfw-context-creator PUBLIC
		${GLFW3_LIBRARY}
		glad)
	target_include_directories(glfw-context-creator PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endif()
