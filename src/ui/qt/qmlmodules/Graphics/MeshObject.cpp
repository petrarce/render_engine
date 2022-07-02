#include "MeshObject.hpp"
#include <GLRenderer>
#include <EigenConversions.hpp>
namespace qmlmodule
{
namespace Graphics
{
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
			if (ambient.canConvert<QColor>())
			{
				auto color = ambient.value<QColor>();
				const auto eigenColor =
					Eigen::Vector4f(color.red() / 256., color.green() / 256.,
									color.blue() / 256., color.alpha() / 265.);
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
	connect(this, &MeshObject::meshChanged, this,
			[this](const QVariant &mesh)
			{
				auto ro = std::reinterpret_pointer_cast<
					dream::renderer::GLMeshWithMaterialObject>(
					mRenderableObject);
				if (mesh.canConvert<QString>())
					ro->setMesh(mesh.value<QString>().toStdString());
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
