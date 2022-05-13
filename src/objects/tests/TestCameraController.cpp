#define BOOST_TEST_MODULE OpengGL wrapper tests
#include <GLGeometryObjects>
#include <GLFW/glfw3.h>
#include <boost/test/included/unit_test.hpp>
#include <GL/osmesa.h>
#include <iostream>
#include <cmath>

using namespace object::camera;

BOOST_AUTO_TEST_CASE(TestCamera)
{
	GLCameraController controller;
	controller.camera =
		GLCamera(Eigen::Vector3f(0, 1, 0), Eigen::Vector3f(1, 0, 0));
	BOOST_TEST(std::fabs(1 - controller.camera.upAxis.squaredNorm()) < 1e-6);
	BOOST_TEST(std::fabs(1 - controller.camera.rightAxis.squaredNorm()) < 1e-6);
	BOOST_TEST(
		std::fabs(controller.camera.upAxis.cross(controller.camera.rightAxis)
					  .squaredNorm() -
				  1) < 1e-6);

	controller.camera.pivot = Eigen::Vector3f(0, 0, 0);
	controller.camera.distance = 10;
	controller.camera.angleX = 0;
	controller.camera.angleZ = 0;
	Eigen::Vector3f translation =
		Eigen::Vector3f(0, 0, 10) -
		controller.camera.transform().block<3, 1>(0, 3);
	BOOST_TEST(translation.squaredNorm() < 1e-8);

	Eigen::Vector3f original = controller.camera.translation();

	controller.cameraRotate(0.123, 0.532);
	auto currentTranslation = controller.camera.translation();
	BOOST_TEST((original - currentTranslation).squaredNorm() > 1e-6);

	controller.cameraRotate(-0.123, -0.532);
	BOOST_TEST((original - controller.camera.translation()).squaredNorm() <
			   1e-6);

	controller.cameraRotate(M_PI_2, 0);
	BOOST_TEST((controller.camera.translation() - Eigen::Vector3f(0, -10, 0))
				   .squaredNorm() < 1e-8);

	controller.cameraRotate(-M_PI_2, 0);
	BOOST_TEST((original - controller.camera.translation()).squaredNorm() <
			   1e-6);

	controller.cameraRotate(0, M_PI_2);
	BOOST_TEST((controller.camera.translation() - Eigen::Vector3f(10, 0, 0))
				   .squaredNorm() < 1e-8);

	controller.cameraRotate(0, -M_PI_2);
	BOOST_TEST((original - controller.camera.translation()).squaredNorm() <
			   1e-6);

	Eigen::Vector4f origin = Eigen::Vector4f(1, 2, 4, 1);
	controller.camera.pivot = Eigen::Vector3f(15, 5, 6);
	controller.camera.angleX = -0.256;
	controller.camera.angleZ = -1.176;
	origin = controller.camera.transform() * origin;
	origin = controller.camera.toViewTransform() * origin;
	BOOST_CHECK((origin - Eigen::Vector4f(1, 2, 4, 1)).squaredNorm() < 1e-6);
}
