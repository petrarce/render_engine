#include "Camera.hpp"
#include <Eigen/Geometry>
#include <iostream>
namespace object
{
namespace camera
{

Eigen::Matrix4f GLCamera::transform() const
{
	Eigen::Vector3f cameraViewAxis;
	cameraViewAxis << upAxis.cross(rightAxis);

	Eigen::AngleAxisf rotationZ(angleZ, upAxis);
	Eigen::AngleAxisf rotationX(angleX, rightAxis);
	Eigen::Matrix3f rotation = (rotationZ * rotationX).matrix();

	Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();
	transform.block<3, 3>(0, 0) = rotation;
	transform.block<3, 1>(0, 3)
		<< pivot + rotation * cameraViewAxis * (-distance);
	return transform;
}

Eigen::Matrix4f GLCamera::toViewTransform() const
{
	auto viewSpaceTransform = transform();
	viewSpaceTransform.block<3, 3>(0, 0).transposeInPlace();
	viewSpaceTransform.block<3, 1>(0, 3)
		<< -1 * viewSpaceTransform.block<3, 3>(0, 0) *
			   viewSpaceTransform.block<3, 1>(0, 3);
	return viewSpaceTransform;
}

Eigen::Matrix3f GLCamera::rotation() const
{
	Eigen::Matrix4f transform = this->transform();
	return transform.block<3, 3>(0, 0);
}

Eigen::Vector3f GLCamera::translation() const
{
	return transform().block<3, 1>(0, 3);
}

} // namespace camera
} // namespace object
