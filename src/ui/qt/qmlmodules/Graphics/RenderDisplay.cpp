#include "RenderDisplay.hpp"

namespace qmlmodule
{
namespace Graphics
{

RenderDisplay::Renderer::Renderer(const RenderDisplay &renderDisplay)
	: QQuickFramebufferObject::Renderer()
	, mRenderDisplay(renderDisplay)
{
}

RenderDisplay::Renderer::~Renderer()
{
}

void RenderDisplay::Renderer::render()
{
	qWarning() << "render executed";
	mRenderDisplay.rootRenderableObject()
		->renderableObject()
		->renderFunction()
		->draw(mRenderingRootScope);
}

void RenderDisplay::Renderer::synchronize(QQuickFramebufferObject *)
{
	mRenderDisplay.rootRenderableObject()->renderableObject()->sync();

	dream::components::Scope newRootScope;
	for (const auto &v : mRenderDisplay.rootScope())
		newRootScope.Set(molecular::util::HashUtils::MakeHash(v.toStdString()),
						 dream::components::Attribute<void>());
	mRenderingRootScope = newRootScope;
}

RenderDisplay::RenderDisplay(QQuickItem *parent)
	: QQuickFramebufferObject(parent)
{
}

RenderDisplay::~RenderDisplay()
{
}

QQuickFramebufferObject::Renderer *RenderDisplay::createRenderer() const
{
	return new RenderDisplay::Renderer(*this);
}

void RenderDisplay::setRootRenderableObject(
	RenderableObject *rootRenderableObject)
{
	if (mRootRenderableObject == rootRenderableObject)
		return;

	mRootRenderableObject = rootRenderableObject;
	connect(mRootRenderableObject, &RenderableObject::update, this,
			&QQuickFramebufferObject::update);
	Q_EMIT rootRenderableObjectChanged(mRootRenderableObject);
}

void RenderDisplay::setRootScope(const QStringList &rootScope)
{
	if (mRootScope == rootScope)
		return;

	mRootScope = rootScope;
	Q_EMIT rootScopeChanged(mRootScope);
}

} // namespace Graphics
} // namespace qmlmodule
