#define BOOST_TEST_MODULE OpengGL wrapper tests
#include <GLGeometryObjects>
#include <GLFW/glfw3.h>
#include <boost/test/included/unit_test.hpp>
#include <GL/osmesa.h>
#include <iostream>

using namespace object::camera;

BOOST_AUTO_TEST_CASE(TestCamera)
{
	GLCameraController controller;
	controller.camera.pivot = Eigen::Vector3f(0, 0, 0);
	controller.camera.distance = 10;
	controller.camera.angleX = 0;
	controller.camera.angleZ = 0;
	std::cout << controller.camera.transform() << std::endl;
	Eigen::Vector3f translation =
		Eigen::Vector3f(0, 0, 10) -
		controller.camera.transform().block<3, 1>(0, 3);
	BOOST_CHECK(translation.squaredNorm() < 1e-8);
}
