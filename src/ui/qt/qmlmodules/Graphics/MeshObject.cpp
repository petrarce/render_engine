#include "MeshObject.hpp"
#include <GLRenderer>
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
			}
			if (ambient.canConvert<QString>())
				ro->setAmbient(
					dream::renderer::GLMeshWithMaterialRenderFunction::Texture{
						ambient.value<QString>().toStdString(),
						dream::glwrapper::GLTexture2D::InternalFormat::
							Srgb8_alpha8 });
		});
	connect(this, &MeshObject::meshChanged, this,
			[this](const QVariant &mesh)
			{
				auto ro = std::reinterpret_pointer_cast<
					dream::renderer::GLMeshWithMaterialObject>(
					mRenderableObject);
				if (mesh.canConvert<QString>())
					ro->setMesh(mesh.value<QString>().toStdString());
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

} // namespace Graphics
} // namespace qmlmodule
