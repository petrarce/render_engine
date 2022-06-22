#include "RenderableObject.hpp"
#include "RenderDisplay.hpp"
namespace qmlmodule
{
namespace Graphics
{
QQmlListProperty<RenderableObject> RenderableObject::renderables() const
{
	return { reinterpret_cast<QObject *>(const_cast<RenderableObject *>(this)),
			 reinterpret_cast<void *>(const_cast<RenderableObject *>(this)),
			 &RenderableObject::appendRenderable,
			 nullptr,
			 nullptr,
			 nullptr };
}

void RenderableObject::appendRenderable(RenderableObject *renderable)
{
	mRenderables.push_back(renderable);
	mRenderableObject->addChild(renderable->renderableObject());
	connect(renderable, &RenderableObject::update, this,
			&RenderableObject::update);
}

void RenderableObject::appendRenderable(
	QQmlListProperty<RenderableObject> *renderablesList,
	RenderableObject *renderable)
{
	reinterpret_cast<RenderableObject *>(renderablesList->data)
		->appendRenderable(renderable);
}

} // namespace Graphics
} // namespace qmlmodule