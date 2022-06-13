add_library(gl-renderer STATIC
	GLRenderableObject.hpp
	GLViewSetupRenderableObject.hpp
	GLCubeObject.hpp
	GLTransformedObject.hpp
	GLRenderer)

target_link_libraries(gl-renderer PUBLIC gl-components)
target_include_directories(gl-renderer PUBLIC .)
if(BUILD_TESTING)
	set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
	add_executable(test-renderer
		tests/TestRenderer.cpp
	)

	target_link_libraries(test-renderer PUBLIC
		boost_unit_test_framework
		gl-renderer
	)

	add_test(NAME test-opengl-components COMMAND test-opengl-components)
endif()

