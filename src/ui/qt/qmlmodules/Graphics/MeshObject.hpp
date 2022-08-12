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

	const QMatrix4x4 &transform() const
	{
		return mTransform;
	}

	RenderMode renderMode() const
	{
		return mRenderMode;
	}

	void setMesh(const QVariant &mesh);
	void setTransform(const QMatrix4x4 &transform);
	void setRenderMode(RenderMode renderMode);

Q_SIGNALS:
	void meshChanged(const QVariant &mesh);
	void transformChanged(const QMatrix4x4 &transfrom);
	void renderModeChanged(RenderMode renderMode);

private:
	bool handleUniform(const QString &name, const QVariant &val) override
	{
		auto ro = std::reinterpret_pointer_cast<
			dream::renderer::GLMeshWithMaterialObject>(mRenderableObject);

		if (name.midRef(0, sizeof("map_") - 1) == "map_" &&
			val.canConvert<QString>())
		{
			dream::renderer::GLMeshWithMaterialObject::Texture map;
			QString mapName = name.mid(sizeof("map_") - 1, name.size());
			map.path		= val.value<QString>().toStdString();

			if (mapName.midRef(0, 4) == "rgb_") // normal map
			{
				map.internalFormat =
					dream::glwrapper::GLTexture2D::InternalFormat::Rgb8;
				mapName = mapName.mid(4);
			}
			else if (mapName.midRef(0, 5) == "srgb_")
			{
				map.internalFormat =
					dream::glwrapper::GLTexture2D::InternalFormat::Srgb8;
				mapName = mapName.mid(5);
			}
			else if (mapName.midRef(0, 5) == "rgba_")
			{
				map.internalFormat =
					dream::glwrapper::GLTexture2D::InternalFormat::Rgba8;
				mapName = mapName.mid(5);
			}
			else if (mapName.midRef(0, 6) == "srgba_")
			{
				map.internalFormat =
					dream::glwrapper::GLTexture2D::InternalFormat::Srgb8_alpha8;
				mapName = mapName.mid(6);
			}
			else if (mapName.midRef(0, 2) == "r_")
			{
				map.internalFormat =
					dream::glwrapper::GLTexture2D::InternalFormat::Red;
				mapName = mapName.mid(2);
			}

			else
				map.internalFormat =
					dream::glwrapper::GLTexture2D::InternalFormat::Rgb8;

			ro->setMap(mapName.toStdString(), map);
			qWarning() << "setting map: " << mapName
					   << QString::fromStdString(map.path);
			return true;
		}
		else
			return RenderableObject::handleUniform(name, val);
	}
	QVariant mMesh;
	QMatrix4x4 mTransform;
	RenderMode mRenderMode{ RenderMode::Faces };
};
} // namespace Graphics
} // namespace qmlmodule
