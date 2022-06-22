#include "RenderDisplay.hpp"

namespace qmlmodule
{
namespace Graphics
{
RenderDisplay::Renderer::Renderer()
	: QQuickFramebufferObject::Renderer()
{
}
RenderDisplay::Renderer::~Renderer()
{
}

void RenderDisplay::Renderer::render()
{
	qWarning() << "render executed";
}
void RenderDisplay::Renderer::synchronize(QQuickFramebufferObject *)
{
	qWarning() << "Synchronize executed";
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
	return new Renderer();
}

} // namespace Graphics
} // namespace qmlmodule
