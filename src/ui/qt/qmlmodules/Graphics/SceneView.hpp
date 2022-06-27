#pragma once
#include <QQuickItem>
#include "RenderableObject.hpp"

namespace qmlmodule
{
namespace Graphics
{

class SceneView : public RenderableObject
{
	Q_OBJECT
	Q_PROPERTY(qreal nearPlane READ nearPlane WRITE setNearPlane NOTIFY
				   nearPlaneChanged)
	Q_PROPERTY(
		qreal farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
	Q_PROPERTY(qreal aspectRatio READ aspectRatio WRITE setAspectRatio NOTIFY
				   aspectRatioChanged)
	Q_PROPERTY(qreal fov READ fov WRITE setFov NOTIFY fovChanged)

public:
	SceneView(QQuickItem *parent = nullptr);
	~SceneView();

	qreal nearPlane() const
	{
		return mNearPlane;
	}
	qreal farPlane() const
	{
		return mFarPlane;
	}
	qreal aspectRatio() const
	{
		return mAspectRatio;
	}
	qreal fov() const
	{
		return mFov;
	}

	void setNearPlane(qreal nearPlane);
	void setFarPlane(qreal farPlane);
	void setAspectRatio(qreal aspectRatio);
	void setFov(qreal fov);

Q_SIGNALS:
	void nearPlaneChanged(qreal nearPlane);
	void farPlaneChanged(qreal farPlane);
	void aspectRatioChanged(qreal aspectRatio);
	void fovChanged(qreal fov);

private:
	qreal mFarPlane{ 0 };
	qreal mAspectRatio{ 0 };
	qreal mFov{ 0 };
	qreal mNearPlane{ 0 };
};

} // namespace Graphics
} // namespace qmlmodule
