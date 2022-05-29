add_library(gl-components STATIC
	GLProgramObject.hpp
	GLProgramObject.cpp
	GLProgramGenerator.hpp
	GLProgramGenerator.cpp
	GLRenderFunction.hpp
	GLArrayBufferDrawer.hpp
	GLArrayBufferDrawer.cpp
	Variable.hpp
	Singleton.hpp
	GLComponents
)

target_include_directories(gl-components PUBLIC .)

target_link_libraries(gl-components PUBLIC
	glad
	molecular-util
	molecular-gfx
	opengl-wrapper-core)

target_deploy_assets(TARGET gl-components ASSETFILES DefaultGlslShader.glslgen)

find_package(OSMesa REQUIRED)

set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
add_executable(test-opengl-components
	tests/TestComponents.cpp
)

target_link_libraries(test-opengl-components PUBLIC
	boost_unit_test_framework
	opengl-wrapper-core
	gl-components
	OSMesa
)

add_test(NAME test-opengl-components COMMAND test-opengl-components)
