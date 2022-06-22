#pragma once
#include <QQuickItem>
#include <GLRenderer>
#include <QQmlListProperty>
#include <QList>
namespace qmlmodule
{
namespace Graphics
{

class RenderableObject : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<RenderableObject> renderables READ renderables)
	Q_CLASSINFO("DefaultProperty", "renderables")
public:
	RenderableObject(QQuickItem *parent = nullptr)
		: QQuickItem(parent)
	{
		// renderable object should be initialized in each subclass
		// according to a implementation
	}
	~RenderableObject()
	{
	}

	std::shared_ptr<dream::renderer::GLRenderableObject>
	renderableObject() const
	{
		return mRenderableObject;
	}

	// qml api
	QQmlListProperty<RenderableObject> renderables() const;

Q_SIGNALS:
	void update();

private:
	void appendRenderable(RenderableObject *renderable);
	static void
	appendRenderable(QQmlListProperty<RenderableObject> *renderablesList,
					 RenderableObject *renderable);

	QList<RenderableObject *> mRenderables;

protected:
	std::shared_ptr<dream::renderer::GLRenderableObject> mRenderableObject;
};

} // namespace Graphics
} // namespace qmlmodule