#pragma once
#include <QQuickItem>
#include <GLRenderer>
#include <QQmlListProperty>
#include <QList>

namespace qmlmodule
{
namespace Graphics
{

class RenderableObject : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<qmlmodule::Graphics::RenderableObject>
				   renderables READ renderables)
	Q_CLASSINFO("DefaultProperty", "renderables")
	QML_ELEMENT
public:
	RenderableObject(QObject *parent = nullptr)
		: QObject(parent)
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

	void appendRenderable(RenderableObject *renderable);
	int countRenderables() const
	{
		return mRenderables.size();
	}
	RenderableObject *renderable(int pos) const
	{
		return mRenderables.at(pos);
	}
	void clearRenderables()
	{
		mRenderables.clear();
	}
	void replaceRenderable(int pos, RenderableObject *renderable)
	{
		mRenderables[pos] = renderable;
	}
	void removeLastRenderable()
	{
		mRenderables.pop_back();
	}

Q_SIGNALS:
	void update();

private:
	static void
	appendRenderable(QQmlListProperty<RenderableObject> *renderablesList,
					 RenderableObject *renderable);
	static int countRenderables(QQmlListProperty<RenderableObject> *);
	static RenderableObject *renderable(QQmlListProperty<RenderableObject> *,
										int pos);
	static void
	clearRenderables(QQmlListProperty<RenderableObject> *renderableList);
	static void replaceRenderable(QQmlListProperty<RenderableObject> *, int pos,
								  RenderableObject *renderable);
	static void removeLastRenderable(QQmlListProperty<RenderableObject> *);

	QList<RenderableObject *> mRenderables;

protected:
	std::shared_ptr<dream::renderer::GLRenderableObject> mRenderableObject;
};

} // namespace Graphics
} // namespace qmlmodule
