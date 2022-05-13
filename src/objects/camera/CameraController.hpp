#pragma once
#include <Eigen/Dense>
#include "Camera.hpp"
#include <iostream>
namespace object
{
namespace camera
{
class GLCameraController
{
public:
	void cameraMove(const Eigen::Vector3f &camSpaceOffset)
	{
		Eigen::Vector4f homogVect;
		homogVect << camSpaceOffset, 1.f;
		Eigen::Vector3f worldSpaceOffset =
			camera.transform().block<3, 3>(0, 0) * camSpaceOffset;
		camera.pivot += worldSpaceOffset;
		std::cout << "Camera moved to " << camera.pivot << std::endl;
	}

	void cameraRotate(float angleXOffset, float andleZOffset)
	{
		camera.angleX += angleXOffset;
		camera.angleZ += andleZOffset;
	}

	void screenSpaceMove(float dx, float dy)
	{
		cameraMove(Eigen::Vector3f(dx, dy, 0));
	}
	void screenSpaceRotate(float dx, float dy)
	{
		cameraRotate(dy, dx);
	}

	GLCamera camera;
};

} // namespace camera
} // namespace object
