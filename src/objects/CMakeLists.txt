find_package(Eigen3 REQUIRED)
add_library(geometry-primitives STATIC
	camera/Camera.cpp
	camera/Camera.hpp
	camera/CameraController.cpp
	camera/CameraController.hpp
	lights/Light.hpp
	meshes/MeshObject.hpp
	GLGeometryObjects)

target_include_directories(geometry-primitives PUBLIC .)

if(BUILD_TESTING)
	add_executable(test-geometry-primitives
		tests/TestCameraController.cpp)

	target_link_libraries(test-geometry-primitives PUBLIC geometry-primitives)
	add_test(NAME test-geometry-promitives COMMAND test-geometry-primitives)
endif()
