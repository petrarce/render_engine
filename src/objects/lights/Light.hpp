#pragma once
#include <Eigen/Dense>
namespace dream
{
namespace geometry
{

class Light
{
public:
	Light()
	{
	}
	virtual ~Light()
	{
	}

	Eigen::Vector3f color() const
	{
		return mColor;
	}
	void setColor(const Eigen::Vector3f &color)
	{
		mColor = color;
	}

protected:
	Eigen::Vector3f mColor;
};

class PointLight : public Light
{
public:
	PointLight()
	{
	}
	virtual ~PointLight()
	{
	}

	Eigen::Vector3f position() const
	{
		return mPosition;
	}
	void setPosition(const Eigen::Vector3f &position)
	{
		mPosition = position;
	}

	float distance() const
	{
		return mDistance;
	}
	void setDistance(float distance)
	{
		mDistance = distance;
	}

protected:
	Eigen::Vector3f mPosition;
	float mDistance;
};

class DirectionLight : public Light
{
public:
	DirectionLight()
	{
	}
	virtual ~DirectionLight()
	{
	}

	Eigen::Vector3f direction() const
	{
		return mDirection;
	}
	void setDirection(const Eigen::Vector3f &direction)
	{
		mDirection = direction;
	}

protected:
	Eigen::Vector3f mDirection;
};

} // namespace geometry

} // namespace dream
