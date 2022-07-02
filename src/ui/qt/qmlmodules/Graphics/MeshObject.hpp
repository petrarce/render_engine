#pragma once
#include "RenderableObject.hpp"
#include <QQuickItem>
#include <QMatrix4x4>
namespace qmlmodule
{
namespace Graphics
{

class MeshObject : public RenderableObject
{
	Q_OBJECT

	Q_PROPERTY(
		QVariant ambient READ ambient WRITE setAmbient NOTIFY ambientChanged)
	Q_PROPERTY(QVariant mesh READ mesh WRITE setMesh NOTIFY meshChanged)
	Q_PROPERTY(QMatrix4x4 transform READ transform WRITE setTransform NOTIFY
				   transformChanged)

public:
	MeshObject(QQuickItem *parent = nullptr);
	~MeshObject();

	const QVariant &mesh() const
	{
		return mMesh;
	}
	const QVariant &ambient() const
	{
		return mAmbient;
	}
	const QMatrix4x4 &transform() const
	{
		return mTransform;
	}

	void setMesh(const QVariant &mesh);
	void setAmbient(const QVariant &ambient);
	void setTransform(const QMatrix4x4 &transform);

Q_SIGNALS:
	void meshChanged(const QVariant &mesh);
	void ambientChanged(const QVariant &ambient);
	void transformChanged(const QMatrix4x4 &transfrom);

private:
	QVariant mAmbient;
	QVariant mMesh;
	QMatrix4x4 mTransform;
};
} // namespace Graphics
} // namespace qmlmodule
