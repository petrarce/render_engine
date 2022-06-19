add_library(gl-renderer STATIC
	GLRenderableObject.hpp
	GLViewSetupRenderableObject.hpp
	GLCubeObject.hpp
	GLTransformedObject.hpp
	GLAssetManager.hpp
	GLAssetManager.cpp
	GLScreenTextureRenderObject.hpp
	GLMaterial.hpp
	GLMaterial.cpp
	GLRenderer)

target_link_libraries(gl-renderer PUBLIC
	gl-components
	stb-lib
	assimp)
target_include_directories(gl-renderer PUBLIC .)

target_deploy_assets(TARGET gl-renderer ASSETFILES
	assets/Grass.png
	assets/TestRGBAImage.png
	assets/Cube.ply
	assets/Triangle.ply
	assets/Monkey.ply)

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
	add_executable(assimp-based-loader
		tests/AssimpBasedLoader.cpp)
	target_link_libraries(assimp-based-loader assimp)
endif()


