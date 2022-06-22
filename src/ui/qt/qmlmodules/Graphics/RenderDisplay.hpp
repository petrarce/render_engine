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
	class Renderer : public QQuickFramebufferObject::Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		void render() override;
		void synchronize(QQuickFramebufferObject *) override;
	};

	RenderDisplay(QQuickItem *parent = nullptr);
	~RenderDisplay();
	QQuickFramebufferObject::Renderer *createRenderer() const override;
};

} // namespace Graphics
} // namespace qmlmodule
