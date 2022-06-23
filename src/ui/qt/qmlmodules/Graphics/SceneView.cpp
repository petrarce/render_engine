#include "SceneView.hpp"
#include <GLRenderer>
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

} // namespace Graphics
} // namespace qmlmodule
