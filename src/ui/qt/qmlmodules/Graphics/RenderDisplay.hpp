#pragma once
#include <QQuickFramebufferObject>
namespace qmlmodule
{
namespace Graphics
{
class RenderDisplay : public QQuickFramebufferObject
{
	Q_OBJECT
public:
	RenderDisplay(QQuickItem *parent = nullptr)
		: QQuickFramebufferObject(parent)
	{
	}
	~RenderDisplay();
	QQuickFramebufferObject::Renderer *createRenderer() const override
	{
		return nullptr;
	}
};
} // namespace Graphics
} // namespace qmlmodule