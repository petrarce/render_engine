#include "Camera.hpp"
#include <EigenConversions.hpp>
namespace qmlmodule
{
namespace Graphics
{
using namespace qt::helpers;

QMatrix4x4 OrbitCamera::viewMatrix()
{
	return toQt(mCameraController.camera.toViewTransform());
}

void OrbitCamera::setPivot(const QVector3D& pivot )
{
	QVector3D diffVec = pivot - toQt(mCameraController.camera.pivot);
	if(qFuzzyIsNull(QVector3D::dotProduct(diffVec, diffVec)))
		return;

	mCameraController.camera.pivot = toEigen(pivot);
	Q_EMIT pivotChanged(pivot);
}

void OrbitCamera::setAngleX(const qreal angleX )
{
	if(qFuzzyCompare(static_cast<float>(angleX), mCameraController.camera.angleX))
		return;

	mCameraController.camera.angleX = angleX;
	Q_EMIT angleXChanged(mCameraController.camera.angleX);
}

void OrbitCamera::setAngleZ(const qreal angleZ )
{
	if(qFuzzyCompare(static_cast<float>(angleZ), mCameraController.camera.angleZ))
		return;

	mCameraController.camera.angleZ = angleZ;
	Q_EMIT angleZChanged(mCameraController.camera.angleZ);
}

void OrbitCamera::setDistance(const qreal distance )
{
	if(qFuzzyCompare(static_cast<float>(distance), mCameraController.camera.distance))
		return;

	mCameraController.camera.distance = distance;
	Q_EMIT distanceChanged(mCameraController.camera.distance);
}

}
}