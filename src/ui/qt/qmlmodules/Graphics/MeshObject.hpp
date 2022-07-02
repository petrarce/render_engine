#pragma once
#include "RenderableObject.hpp"
#include <QQuickItem>
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

	void setMesh(const QVariant &mesh);
	void setAmbient(const QVariant &ambient);

Q_SIGNALS:
	void meshChanged(const QVariant &mesh);
	void ambientChanged(const QVariant &ambient);

private:
	QVariant mAmbient;
	QVariant mMesh;
};
} // namespace Graphics
} // namespace qmlmodule
