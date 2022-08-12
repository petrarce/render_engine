#pragma once
#include <Eigen/Dense>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
namespace qt
{
namespace helpers
{

inline Eigen::Matrix4f toEigen(const QMatrix4x4& qmat)
{
	Eigen::Matrix4f emat;
	emat <<	qmat(0, 0), qmat(0, 1), qmat(0,2), qmat(0,3),
			qmat(1, 0), qmat(1, 1), qmat(1,2), qmat(1,3),
			qmat(2, 0), qmat(2, 1), qmat(2,2), qmat(2,3),
			qmat(3, 0), qmat(3, 1), qmat(3,2), qmat(3,3);
	return emat;
}

inline Eigen::Matrix3f toEigen(const QMatrix3x3& qmat)
{
	Eigen::Matrix3f emat;
	emat <<	qmat(0, 0), qmat(0, 1), qmat(0,2),
			qmat(1, 0), qmat(1, 1), qmat(1,2),
			qmat(2, 0), qmat(2, 1), qmat(2,2);
	return emat;
}

inline Eigen::Vector2f toEigen(const QVector2D &qvec)
{
	Eigen::Vector2f evec;
	evec << qvec.x(), qvec.y();
	return evec;
}

inline Eigen::Vector3f toEigen(const QVector3D& qvec)
{
	Eigen::Vector3f evec;
	evec << qvec.x(), qvec.y(), qvec.z();
	return evec;
}

inline Eigen::Vector4f toEigen(const QVector4D &qvec)
{
	Eigen::Vector4f evec;
	evec << qvec.x(), qvec.y(), qvec.z(), qvec.w();
	return evec;
}

inline QMatrix4x4 toQt(const Eigen::Matrix4f& emat)
{
	QMatrix4x4 qmat(
				emat(0, 0), emat(0, 1), emat(0, 2), emat(0, 3),
				emat(1, 0), emat(1, 1), emat(1, 2), emat(1, 3),
				emat(2, 0), emat(2, 1), emat(2, 2), emat(2, 3),
				emat(3, 0), emat(3, 1), emat(3, 2), emat(3, 3)
	);
	return qmat;
}

inline QMatrix3x3 toQt(const Eigen::Matrix3f& emat)
{
	QMatrix3x3 qmat;
	
	qmat(0, 0) = emat(0, 0); qmat(0, 1) = emat(0, 1); qmat(0, 2) =  emat(0, 2);
	qmat(1, 0) = emat(1, 0); qmat(1, 1) = emat(1, 1); qmat(1, 2) =  emat(1, 2);
	qmat(2, 0) = emat(2, 0); qmat(2, 1) = emat(2, 1); qmat(2, 2) =  emat(2, 2);

	return qmat;
}

inline QVector3D toQt(const Eigen::Vector3f& evec)
{
	return QVector3D(evec(0), evec(1), evec(2));
}

} // namespace helpers
} // namespace qt
