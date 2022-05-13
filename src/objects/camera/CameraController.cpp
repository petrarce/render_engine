#include <Eigen/Dense>
#include "CameraController.hpp"
namespace object
{
namespace camera
{
void GLCameraController::cameraMove(const Eigen::Vector3f &camSpaceOffset)
{
	Eigen::Vector4f homogVect;
	homogVect << camSpaceOffset, 1.f;
	Eigen::Vector3f worldSpaceOffset =
		camera.transform().block<3, 3>(0, 0) * camSpaceOffset;
	camera.pivot += worldSpaceOffset;
}

void GLCameraController::cameraRotate(float angleXOffset, float andleZOffset)
{
	camera.angleX += angleXOffset;
	camera.angleZ += andleZOffset;
}

} // namespace camera
} // namespace object
