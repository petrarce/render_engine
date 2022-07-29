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
	Q_PROPERTY(QUrl normalMap READ normalMap WRITE setNormalMap NOTIFY
				   normalMapChanged)
	Q_PROPERTY(QVariant mesh READ mesh WRITE setMesh NOTIFY meshChanged)
	Q_PROPERTY(QMatrix4x4 transform READ transform WRITE setTransform NOTIFY
				   transformChanged)
	Q_PROPERTY(RenderMode renderMode READ renderMode WRITE setRenderMode NOTIFY
				   renderModeChanged)

public:
	enum RenderMode
	{
		Faces,
		Lines,
		Points,
		FacesLines,
		FacesPoints,
		LinesPoints,
		FacesLinesPoints
	};
	Q_ENUM(RenderMode)

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
	const QUrl normalMap() const
	{
		return mNormalMap;
	}
	const QMatrix4x4 &transform() const
	{
		return mTransform;
	}

	RenderMode renderMode() const
	{
		return mRenderMode;
	}

	void setMesh(const QVariant &mesh);
	void setAmbient(const QVariant &ambient);
	void setNormalMap(const QUrl &url);
	void setTransform(const QMatrix4x4 &transform);
	void setRenderMode(RenderMode renderMode);

Q_SIGNALS:
	void meshChanged(const QVariant &mesh);
	void ambientChanged(const QVariant &ambient);
	void normalMapChanged(const QUrl &normalMap);
	void transformChanged(const QMatrix4x4 &transfrom);
	void renderModeChanged(RenderMode renderMode);

private:
	QVariant mAmbient;
	QUrl mNormalMap;
	QVariant mMesh;
	QMatrix4x4 mTransform;
	RenderMode mRenderMode{ RenderMode::Faces };
};
} // namespace Graphics
} // namespace qmlmodule
