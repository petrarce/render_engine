#pragma once
#include <QQuickItem>
#include <QMatrix4x4>
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
	Q_PROPERTY(QMatrix4x4 viewMatrix READ viewMatrix WRITE setViewMatrix NOTIFY
				   viewMatrixChanged)

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

	const QMatrix4x4 &viewMatrix() const
	{
		return mViewMatrix;
	}

	void setNearPlane(qreal nearPlane);
	void setFarPlane(qreal farPlane);
	void setAspectRatio(qreal aspectRatio);
	void setFov(qreal fov);
	void setViewMatrix(const QMatrix4x4 &viewMatrix);

Q_SIGNALS:
	void nearPlaneChanged(qreal nearPlane);
	void farPlaneChanged(qreal farPlane);
	void aspectRatioChanged(qreal aspectRatio);
	void fovChanged(qreal fov);
	void viewMatrixChanged(const QMatrix4x4 &viewMatrix);

private:
	qreal mFarPlane{ 0 };
	qreal mAspectRatio{ 0 };
	qreal mFov{ 0 };
	qreal mNearPlane{ 0 };
	QMatrix4x4 mViewMatrix;
};

} // namespace Graphics
} // namespace qmlmodule
