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
		->draw();
}
void RenderDisplay::Renderer::synchronize(QQuickFramebufferObject *)
{
	qWarning() << "Synchronize executed";
	mRenderDisplay.rootRenderableObject()->renderableObject()->sync();
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

} // namespace Graphics
} // namespace qmlmodule
