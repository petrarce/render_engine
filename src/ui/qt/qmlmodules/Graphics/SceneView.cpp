#include "SceneView.hpp"
#include "Lights.hpp"
#include <GLRenderer>
#include <EigenConversions.hpp>
namespace qmlmodule
{
namespace Graphics
{

SceneView::SceneView(QQuickItem *parent)
	: RenderableObject(parent)
{
	mRenderableObject =
		std::make_shared<dream::renderer::GLViewSetupRenderableObject>();
	connect(this, &SceneView::nearPlaneChanged,
			[this](qreal nearPlane)
			{
				auto renderable = std::reinterpret_pointer_cast<
					dream::renderer::GLViewSetupRenderableObject>(
					mRenderableObject);
				renderable->setNearPlane(nearPlane);
				Q_EMIT update();
			});
	connect(this, &SceneView::farPlaneChanged,
			[this](qreal farPlane)
			{
				auto renderable = std::reinterpret_pointer_cast<
					dream::renderer::GLViewSetupRenderableObject>(
					mRenderableObject);
				renderable->setFarPlane(farPlane);
				Q_EMIT update();
			});
	connect(this, &SceneView::aspectRatioChanged,
			[this](qreal ar)
			{
				auto renderable = std::reinterpret_pointer_cast<
					dream::renderer::GLViewSetupRenderableObject>(
					mRenderableObject);
				renderable->setAspectRatio(ar);
				Q_EMIT update();
			});
	connect(this, &SceneView::fovChanged,
			[this](qreal fov)
			{
				auto renderable = std::reinterpret_pointer_cast<
					dream::renderer::GLViewSetupRenderableObject>(
					mRenderableObject);
				renderable->setFov(fov);
				Q_EMIT update();
			});
	connect(this, &SceneView::viewMatrixChanged,
			[this](const QMatrix4x4 &view)
			{
				auto renderable = std::reinterpret_pointer_cast<
					dream::renderer::GLViewSetupRenderableObject>(
					mRenderableObject);
				renderable->setViewTransform(qt::helpers::toEigen(view));
				Q_EMIT update();
			});
	connect(
		this, &SceneView::lightsChanged,
		[this](const QVariantList &lights)
		{
			auto renderable = std::reinterpret_pointer_cast<
				dream::renderer::GLViewSetupRenderableObject>(
				mRenderableObject);
			dream::renderer::GLViewSetupRenderableObject::LightsVec lightsVec;
			for (auto l : lights)
			{

				if (l.canConvert<QVariantMap>())
				{
					auto lightProperties = l.value<QVariantMap>();
					if (lightProperties.keys().contains("color"))
					{
						auto color = qt::helpers::toEigen(
							lightProperties.value("color").value<QVector3D>());
						// directional light
						if (lightProperties.keys().contains("direction"))
						{
							auto direction = qt::helpers::toEigen(
								lightProperties.value("direction")
									.value<QVector3D>());
							auto light = std::make_shared<
								dream::geometry::DirectionLight>();
							light->setColor(color);
							light->setDirection(direction);
							lightsVec.push_back(light);
						}
						// point light
						else if (lightProperties.keys().contains("position"))
						{
							auto position = qt::helpers::toEigen(
								lightProperties.value("position")
									.value<QVector3D>());
							auto light =
								std::make_shared<dream::geometry::PointLight>();
							light->setColor(color);
							light->setPosition(position);
							lightsVec.push_back(light);
						}
					}
				}
			}
			renderable->setLights(lightsVec);
			Q_EMIT update();
		});
}

SceneView::~SceneView()
{
}

void SceneView::setNearPlane(qreal nearPlane)
{
	if (qFuzzyCompare(mNearPlane, nearPlane))
		return;

	mNearPlane = nearPlane;
	Q_EMIT nearPlaneChanged(mNearPlane);
}

void SceneView::setFarPlane(qreal farPlane)
{
	if (qFuzzyCompare(mFarPlane, farPlane))
		return;

	mFarPlane = farPlane;
	Q_EMIT farPlaneChanged(mFarPlane);
}

void SceneView::setAspectRatio(qreal aspectRatio)
{
	if (qFuzzyCompare(mAspectRatio, aspectRatio))
		return;

	mAspectRatio = aspectRatio;
	Q_EMIT aspectRatioChanged(mAspectRatio);
}

void SceneView::setFov(qreal fov)
{
	if (qFuzzyCompare(mFov, fov))
		return;

	mFov = fov;
	Q_EMIT fovChanged(mFov);
}

void SceneView::setViewMatrix(const QMatrix4x4 &viewMatrix)
{
	if (mViewMatrix == viewMatrix)
		return;

	mViewMatrix = viewMatrix;
	Q_EMIT viewMatrixChanged(mViewMatrix);
}

void SceneView::setLights(const QVariantList &lights)
{
	if (lights == mLights)
		return;

	mLights = lights;
	Q_EMIT lightsChanged(lights);
}

} // namespace Graphics
} // namespace qmlmodule
