add_library(opengl-wrapper-core STATIC
	GLObjectBinder.hpp
	GLObjectBinder.cpp
	GLObject.hpp
	GLBuffer.hpp
	GLVertexArray.hpp
	GLProgram.hpp
	GLProgram.cpp
	GLShader.hpp
	GLWrapperCore
	GLDefinitions.hpp
	GLTexture.hpp
)

target_link_libraries(opengl-wrapper-core PUBLIC glad)
target_include_directories(opengl-wrapper-core PUBLIC .)

# Unit tests for opengl wrapper
find_package(OSMesa REQUIRED)
set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
add_executable(test-opengl-wrapper-core
	tests/TestGlWrapper.cpp
)

target_link_libraries(test-opengl-wrapper-core PUBLIC
	boost_unit_test_framework
	OSMesa
	opengl-wrapper-core
)

add_test(NAME test-opengl-wrapper-core COMMAND test-opengl-wrapper-core)
