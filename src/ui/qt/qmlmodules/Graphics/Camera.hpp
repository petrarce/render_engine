#pragma once
#include <QObject>
#include <QQuickItem>
#include <QMatrix4x4>
#include <EigenConversions.hpp>
#include <src/objects/camera/CameraController.hpp>
namespace qmlmodule
{
namespace Graphics
{

class Camera : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QMatrix4x4 viewMatrix READ viewMatrix NOTIFY viewMatrixChanged)
public:
	Camera(QObject *parent = nullptr)
		: QObject(parent)
	{
	}
	virtual ~Camera()
	{
	}

	virtual QMatrix4x4 viewMatrix()				  = 0;
	virtual void moveCamera(float dx, float dy)	  = 0;
	virtual void rotateCamera(float dx, float dy) = 0;
	virtual void zoomCamera(float dz)			  = 0;

Q_SIGNALS:
	void viewMatrixChanged(const QMatrix4x4 &view);
};

class OrbitCamera : public Camera
{
	Q_OBJECT
	Q_PROPERTY(QVector3D pivot READ pivot WRITE setPivot NOTIFY pivotChanged)
	Q_PROPERTY(qreal angleX READ angleX WRITE setAngleX NOTIFY angleXChanged)
	Q_PROPERTY(qreal angleZ READ angleZ WRITE setAngleZ NOTIFY angleZChanged)
	Q_PROPERTY(
		qreal distance READ distance WRITE setDistance NOTIFY distanceChanged)

public:
	OrbitCamera(QObject *parent = nullptr)
		: Camera(parent)
	{
		mCameraController.camera.angleX	   = -45;
		mCameraController.camera.angleZ	   = 0;
		mCameraController.camera.distance  = 20;
		mCameraController.camera.pivot	   = Eigen::Vector3f(0, 0, 0);
		mCameraController.camera.upAxis	   = Eigen::Vector3f(0, 0, 1);
		mCameraController.camera.rightAxis = Eigen::Vector3f(1, 0, 0);
		connect(this, &OrbitCamera::pivotChanged, this,
				[this]()
				{
					viewMatrixChanged(qt::helpers::toQt(
						mCameraController.camera.toViewTransform()));
				});
		connect(this, &OrbitCamera::angleXChanged, this,
				[this]()
				{
					viewMatrixChanged(qt::helpers::toQt(
						mCameraController.camera.toViewTransform()));
				});
		connect(this, &OrbitCamera::angleZChanged, this,
				[this]()
				{
					viewMatrixChanged(qt::helpers::toQt(
						mCameraController.camera.toViewTransform()));
				});
		connect(this, &OrbitCamera::distanceChanged, this,
				[this]()
				{
					viewMatrixChanged(qt::helpers::toQt(
						mCameraController.camera.toViewTransform()));
				});
	}

	virtual ~OrbitCamera()
	{
	}

	QMatrix4x4 viewMatrix() override;

	const QVector3D pivot() const
	{
		return qt::helpers::toQt(mCameraController.camera.pivot);
	}
	qreal angleX() const
	{
		return mCameraController.camera.angleX;
	}
	qreal angleZ() const
	{
		return mCameraController.camera.angleZ;
	}
	qreal distance() const
	{
		return mCameraController.camera.distance;
	}

	void setPivot(const QVector3D &pivot);
	void setAngleX(const qreal angleX);
	void setAngleZ(const qreal angleZ);
	void setDistance(const qreal distance);

	void moveCamera(float dx, float dy) override
	{
		QVector2D offset(dx, dy);
		if (qFuzzyIsNull(QVector3D::dotProduct(offset, offset)))
			return;

		mCameraController.cameraMove(Eigen::Vector3f(dx, dy, 0));
		Q_EMIT pivotChanged(pivot());
	}

	void rotateCamera(float dx, float dy) override
	{
		QVector2D offset(dx, dy);
		if (qFuzzyIsNull(QVector3D::dotProduct(offset, offset)))
			return;

		mCameraController.cameraRotate(dy, dx);
		Q_EMIT angleZChanged(angleZ());
		Q_EMIT angleXChanged(angleZ());
	}

	void zoomCamera(float dz) override
	{
		if (qFuzzyIsNull(dz))
			return;
		mCameraController.camera.distance += dz;
		Q_EMIT distanceChanged(distance());
	}

Q_SIGNALS:
	void pivotChanged(const QVector3D &pivot);
	void angleXChanged(const qreal angleX);
	void angleZChanged(const qreal angleZ);
	void distanceChanged(const qreal distance);

private:
	dream::geometry::GLCameraController mCameraController;
};
} // namespace Graphics
} // namespace qmlmodule
