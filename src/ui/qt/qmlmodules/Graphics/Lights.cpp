#include "Lights.hpp"
namespace qmlmodule
{

namespace Graphics
{

void Light::setColor(const QVector3D &color)
{
	if (mColor == color)
		return;

	mColor = color;
	Q_EMIT colorChanged(mColor);
}

void DirectionLight::setDirection(const QVector3D &direction)
{
	if (mDirection == direction)
		return;

	mDirection = direction;
	Q_EMIT directionChanged(mDirection);
}

void PointLight::setPosition(const QVector3D &position)
{
	if (mPosition == position)
		return;

	mPosition = position;
	Q_EMIT positionChanged(mPosition);
}

void PointLight::setAttenuationDistance(qreal attenuationDistance)
{
	if (qFuzzyCompare(attenuationDistance, mAttenuationDistance))
		return;

	mAttenuationDistance = attenuationDistance;
	Q_EMIT attenuationDistanceChanged(mAttenuationDistance);
}

} // namespace Graphics
} // namespace qmlmodule
