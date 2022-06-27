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
		->draw(*mRenderingRootScope);
}

void RenderDisplay::Renderer::synchronize(QQuickFramebufferObject *)
{
	mRenderDisplay.rootRenderableObject()->renderableObject()->sync();

	auto newRootScope = std::make_unique<dream::components::Scope>();
	for (const auto &v : mRenderDisplay.mRootScope)
		newRootScope->Set(molecular::util::HashUtils::MakeHash(v.toStdString()),
						  dream::components::Attribute<void>());
	mRenderingRootScope = std::move(newRootScope);
}

RenderDisplay::RenderDisplay(QQuickItem *parent)
	: QQuickFramebufferObject(parent)
	, mRootScope({ "fragColor", "gl_Position" })
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

	disconnect(mRootRenderableObject, &RenderableObject::update, this,
			   &QQuickFramebufferObject::update);
	mRootRenderableObject = rootRenderableObject;
	connect(mRootRenderableObject, &RenderableObject::update, this,
			&QQuickFramebufferObject::update);
	Q_EMIT rootRenderableObjectChanged(mRootRenderableObject);
}

void RenderDisplay::setMode(RenderingMode mode)
{
	if (mMode == mode)
		return;

	mMode = mode;
	Q_EMIT modeChanged(mMode);
}

} // namespace Graphics
} // namespace qmlmodule
