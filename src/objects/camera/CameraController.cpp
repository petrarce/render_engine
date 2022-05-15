#include <Eigen/Dense>
#include "CameraController.hpp"
namespace dream
{
namespace geometry
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

} // namespace geometry
} // namespace dream
