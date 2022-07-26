#pragma once
#include <QObject>
#include <QVector3D>
namespace qmlmodule
{

namespace Graphics
{

class Light : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QVector3D color READ color WRITE setColor NOTIFY colorChanged)

public:
	explicit Light(QObject *parent = nullptr)
		: QObject(parent)
	{
	}
	virtual ~Light()
	{
	}

	QVector3D color() const
	{
		return mColor;
	}

	void setColor(const QVector3D &color);

Q_SIGNALS:
	void colorChanged(const QVector3D &color);

private:
	QVector3D mColor;
};

class DirectionLight : public Light
{
	Q_OBJECT
	Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY
				   directionChanged)
public:
	explicit DirectionLight(QObject *parent = nullptr)
		: Light(parent)
	{
	}

	virtual ~DirectionLight()
	{
	}

	QVector3D direction() const
	{
		return mDirection;
	}
	void setDirection(const QVector3D &direction);

Q_SIGNALS:
	void directionChanged(const QVector3D &direction);

private:
	QVector3D mDirection;
};

class PointLight : public Light
{
	Q_OBJECT
	Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY
				   positionChanged)
	Q_PROPERTY(qreal attenuationDistance READ attenuationDistance WRITE
				   setAttenuationDistance NOTIFY attenuationDistanceChanged)
public:
	explicit PointLight(QObject *parent = nullptr)
		: Light(parent)
	{
	}

	virtual ~PointLight()
	{
	}

	QVector3D position() const
	{
		return mPosition;
	}

	qreal attenuationDistance() const
	{
		return mAttenuationDistance;
	}

	void setPosition(const QVector3D &direction);
	void setAttenuationDistance(qreal attenuationDistance);

Q_SIGNALS:
	void positionChanged(const QVector3D &direction);
	void attenuationDistanceChanged(qreal attenuationDistance);

private:
	QVector3D mPosition;
	qreal mAttenuationDistance;
};
} // namespace Graphics
} // namespace qmlmodule
