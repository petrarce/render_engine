#pragma once
#include <QQuickItem>
#include <GLRenderer>
#include <QQmlListProperty>
#include <QQmlParserStatus>
#include <QList>

namespace qmlmodule
{
namespace Graphics
{

class RenderableObject : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QQmlListProperty<qmlmodule::Graphics::RenderableObject>
				   renderables READ renderables)
	Q_CLASSINFO("DefaultProperty", "renderables")
	QML_ELEMENT
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

	Q_INVOKABLE void addRenderableObject(RenderableObject *renderable)
	{
		appendRenderable(renderable);
	}

	void classBegin() override;
	void componentComplete() override;

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

	void uniformChanged()
	{
		auto mo = this->metaObject();
		for (int i = 0; i < mo->propertyCount(); i++)
		{
			auto property = mo->property(i);
			QString name(property.name());
			QVariant value = property.read(this);
			if (mUniforms.contains(name) && value != mUniforms.value(name))
				uniformChanged(name.mid(sizeof("u_") - 1), value);
		}
	}

Q_SIGNALS:
	void update();
	void uniformChanged(const QString &name, const QVariant &value);

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

	QMap<QString, QVariant> mUniforms;

protected:
	std::shared_ptr<dream::renderer::GLRenderableObject> mRenderableObject;
};

} // namespace Graphics
} // namespace qmlmodule
