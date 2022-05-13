#pragma once
#include <Eigen/Dense>
namespace object
{
namespace camera
{

class GLCamera
{
public:
	Eigen::Vector3f pivot = Eigen::Vector3f(0, 0, 0);
	float angleX{0};
	float angleZ{0};
	float distance{10};
	const Eigen::Vector3d upAxis = Eigen::Vector3f(0, 1, 0);
	const Eigen::Vector3d rightAxis = Eigen::Vector3f(1, 0, 0);

	GLCamera(const Eigen::Vector3f &ua = Eigen::Vector3f(0, 1, 0),
			 const Eigen::Vector3f &ra = Eigen::Vector3f(1, 0, 0))
		: upAxis(ua.normalized())
		, rightAxis(ra.cross(ua).cross(ra).normalized())
	{
	}
	Eigen::Matrix4f transform() const;
	Eigen::Matrix4f toViewTransform() const;

private:
};

} // namespace camera
} // namespace object
