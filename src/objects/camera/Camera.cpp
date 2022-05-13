#include "Camera.hpp"
#include <Eigen/Geometry>
#include <iostream>
namespace object
{
namespace camera
{

Eigen::Matrix4f GLCamera::transform() const
{
	Eigen::Translation3f translation(pivot);
	Eigen::AngleAxisf rotationZ(angleZ, upAxis);
	Eigen::AngleAxisf rotationX(angleX, rightAxis);
	Eigen::Translation3f offset(Eigen::Vector3f(0, 0, distance));
	Eigen::Matrix4f transform;
	transform.block<3, 3>(0, 0) = (rotationX * rotationZ).matrix();
	transform.block<4, 1>(0, 3) =
		(offset * rotationX * rotationZ * translation).matrix() *
		Eigen::Vector4f(0, 0, 0, 1);
	return transform;
}

Eigen::Matrix4f GLCamera::toViewTransform() const
{
	auto viewSpaceTransform = transform();
	viewSpaceTransform.block<0, 0>(3, 3).transposeInPlace();
	viewSpaceTransform.block<3, 1>(0, 3)
		<< -viewSpaceTransform.block<3, 1>(0, 3);
	return viewSpaceTransform;
}

} // namespace camera
} // namespace object
