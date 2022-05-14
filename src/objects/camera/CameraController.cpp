#include <Eigen/Dense>
#include "CameraController.hpp"
namespace object
{
namespace camera
{
void GLCameraController::cameraMove(const Eigen::Vector3f &camSpaceOffset)
{
	camera.pivot += camera.rotation() * camSpaceOffset;
}

void GLCameraController::cameraRotate(float angleXOffset, float andleZOffset)
{
	camera.angleX += angleXOffset;
	camera.angleZ += andleZOffset;
}

} // namespace camera
} // namespace object
