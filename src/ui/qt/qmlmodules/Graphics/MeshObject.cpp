#include "MeshObject.hpp"
#include <GLRenderer>
#include <EigenConversions.hpp>
namespace qt
{
namespace helpers
{

template <class T,
		  typename std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
inline T toEigen(const T &val)
{
	return val;
}

} // namespace helpers
} // namespace qt

namespace qmlmodule
{
namespace Graphics
{

template <class QtType, class TargetBufferType>
void deployBuffer(const QString &field, const QVariantMap &meshMap,
				  TargetBufferType &originalBuffer)
{
	if (meshMap.contains(field))
	{
		auto vertices = meshMap.value(field);
		if (vertices.canConvert<QVariantList>())
		{
			auto verticesList = vertices.value<QVariantList>();
			for (const auto &v : verticesList)
				if (v.canConvert<QtType>())
					originalBuffer.push_back(
						qt::helpers::toEigen(v.value<QtType>()));
		}
	}
}

MeshObject::MeshObject(QQuickItem *parent)
	: RenderableObject(parent)
{
	auto ro = std::make_shared<dream::renderer::GLMeshWithMaterialObject>();
	mRenderableObject =
		std::reinterpret_pointer_cast<dream::renderer::GLRenderableObject>(ro);
	connect(
		this, &MeshObject::ambientChanged, this,
		[this](const QVariant &ambient)
		{
			auto ro = std::reinterpret_pointer_cast<
				dream::renderer::GLMeshWithMaterialObject>(mRenderableObject);
			if (ambient.canConvert<QColor>() ||
				(ambient.canConvert<QString>() &&
				 QColor::isValidColor(ambient.value<QString>())))
			{
				auto color = ambient.value<QColor>();
				const auto eigenColor =
					Eigen::Vector4f(color.red() / 255., color.green() / 255.,
									color.blue() / 255., color.alpha() / 255.);
				ro->setAmbient(eigenColor);
				update();
			}
			else if (ambient.canConvert<QString>())
			{
				ro->setAmbient(
					dream::renderer::GLMeshWithMaterialRenderFunction::Texture{
						ambient.value<QString>().toStdString(),
						dream::glwrapper::GLTexture2D::InternalFormat::
							Srgb8_alpha8 });
				update();
			}
		});
	connect(
		this, &MeshObject::meshChanged, this,
		[this](const QVariant &mesh)
		{
			auto ro = std::reinterpret_pointer_cast<
				dream::renderer::GLMeshWithMaterialObject>(mRenderableObject);
			if (mesh.canConvert<QString>())
				ro->setMesh(mesh.value<QString>().toStdString());
			else if (mesh.canConvert<QVariantMap>())
			{
				auto newMesh = std::make_shared<dream::geometry::GLMesh>();
				auto meshMap = mesh.value<QVariantMap>();
				deployBuffer<QVector3D>("vertices", meshMap,
										newMesh->vertices());
				deployBuffer<QVector3D>("normals", meshMap, newMesh->normals());
				deployBuffer<QVector3D>("tangents", meshMap,
										newMesh->tangents());
				deployBuffer<QVector3D>("bitangents", meshMap,
										newMesh->bitangents());
				deployBuffer<QVector2D>("texCoord", meshMap,
										newMesh->texCoord());
				deployBuffer<int>("indices", meshMap, newMesh->indices());
				ro->setMesh(newMesh);
			}
			update();
		});
	connect(this, &MeshObject::transformChanged, this,
			[this](const QMatrix4x4 &transform)
			{
				auto ro = std::reinterpret_pointer_cast<
					dream::renderer::GLMeshWithMaterialObject>(
					mRenderableObject);
				ro->setTransform(qt::helpers::toEigen(mTransform));
				update();
			});
	connect(
		this, &MeshObject::renderModeChanged, this,
		[this](RenderMode renderMode)
		{
			auto ro = std::reinterpret_pointer_cast<
				dream::renderer::GLMeshWithMaterialObject>(mRenderableObject);
			if (renderMode == RenderMode::Faces)
				ro->setRenderMode(dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Faces);
			else if (renderMode == RenderMode::Lines)
				ro->setRenderMode(dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Lines);
			else if (renderMode == RenderMode::Points)
				ro->setRenderMode(dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Points);
			else if (renderMode == RenderMode::FacesLines)
				ro->setRenderMode(dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Lines |
								  dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Faces);
			else if (renderMode == RenderMode::FacesPoints)
				ro->setRenderMode(dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Points |
								  dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Faces);
			else if (renderMode == RenderMode::LinesPoints)
				ro->setRenderMode(dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Lines |
								  dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Points);
			else if (renderMode == RenderMode::FacesLinesPoints)
				ro->setRenderMode(dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Lines |
								  dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Points |
								  dream::renderer::GLMeshWithMaterialObject::
									  RenderMode::Faces);
		});
	connect(this, &MeshObject::normalMapChanged, this,
			[this](const QUrl &normalMap)
			{
				auto ro = std::reinterpret_pointer_cast<
					dream::renderer::GLMeshWithMaterialObject>(
					mRenderableObject);
				dream::renderer::GLMeshWithMaterialObject::Texture texture = {
					.path = normalMap.path().toStdString(),
					.internalFormat =
						dream::glwrapper::GLTexture2D::InternalFormat::Rgb8,
				};
				ro->setMap("uNormalMapTexture", texture);
				update();
			});
	connect(this, 
			qOverload<const QString&, const QVariant&>(&RenderableObject::uniformChanged), 
			this, 
			[](const QString& name, const QVariant& value)
	{
		auto ro = std::reinterpret_pointer_cast<
			dream::renderer::GLMeshWithMaterialObject>(mRenderableObject);
		if (name.midRef(0, sizeof("map_") - 1) == "map_")
		{
			QString mapName = name.mid(0, sizeof("comp-1") - 1);
			if (mapName.midRef(0, 2) == "srgb") // normal map
			{
			}
		});
}
MeshObject::~MeshObject()
{
}

void MeshObject::setMesh(const QVariant &mesh)
{
	if (mesh == mMesh)
		return;

	mMesh = mesh;
	Q_EMIT meshChanged(mMesh);
}
void MeshObject::setAmbient(const QVariant &ambient)
{
	if (ambient == mAmbient)
		return;

	mAmbient = ambient;
	Q_EMIT ambientChanged(mAmbient);
}

void MeshObject::setNormalMap(const QUrl &url)
{
	if (mNormalMap == url)
		return;

	mNormalMap = url;
	Q_EMIT normalMapChanged(mNormalMap);
}

void MeshObject::setTransform(const QMatrix4x4 &transform)
{
	if (mTransform == transform)
		return;

	mTransform = transform;
	Q_EMIT transformChanged(mTransform);
}

void MeshObject::setRenderMode(RenderMode renderMode)
{
	if (mRenderMode == renderMode)
		return;

	mRenderMode = renderMode;
	Q_EMIT renderModeChanged(mRenderMode);
}

} // namespace Graphics
} // namespace qmlmodule
