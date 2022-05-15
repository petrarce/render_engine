#pragma once
#include <Eigen/Dense>
namespace dream
{
namespace geometry
{

class GLCamera
{
public:
	GLCamera &operator=(const GLCamera &other) = default;

	GLCamera(const Eigen::Vector3f &ua = Eigen::Vector3f(0, 1, 0),
			 const Eigen::Vector3f &ra = Eigen::Vector3f(1, 0, 0))
		: upAxis(ua.normalized())
		, rightAxis(ua.cross(ra.cross(ua)).normalized())
	{
	}
	Eigen::Matrix4f transform() const;
	Eigen::Matrix4f toViewTransform() const;
	Eigen::Matrix3f rotation() const;
	Eigen::Vector3f translation() const;

	Eigen::Vector3f pivot = Eigen::Vector3f(0, 0, 0);
	float angleX{ 0 };
	float angleZ{ 0 };
	float distance{ 10 };
	Eigen::Vector3f upAxis	  = Eigen::Vector3f(0, 1, 0);
	Eigen::Vector3f rightAxis = Eigen::Vector3f(1, 0, 0);
};

} // namespace geometry
} // namespace dream
