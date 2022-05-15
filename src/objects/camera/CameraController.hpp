#pragma once
#include <Eigen/Dense>
#include "Camera.hpp"
#include <iostream>
namespace dream
{
namespace geometry
{
class GLCameraController
{
public:
	void cameraMove(const Eigen::Vector3f &camSpaceOffset);

	void cameraRotate(float angleXOffset, float andleZOffset);

	GLCamera camera;
};

} // namespace geometry
} // namespace dream
