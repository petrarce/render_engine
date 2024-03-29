#include "RenderableObject.hpp"
#include "RenderDisplay.hpp"
#include <QRegExp>
namespace qmlmodule
{
namespace Graphics
{

void RenderableObject::classBegin()
{
}

void RenderableObject::componentComplete()
{
	const QMetaObject *mo = this->metaObject();
	if (!mo)
		return;

	connect(this,
			qOverload<const QString &, const QVariant &>(
				&RenderableObject::uniformChanged),
			this,
			[this](const QString &name, const QVariant &val)
			{ this->handleUniform(name, val); });

	for (int i = 0; i < mo->propertyCount(); i++)
	{
		const auto &property = mo->property(i);
		QString name(property.name());
		if (name.mid(0, sizeof("u_") - 1) == "u_")
		{
			QVariant value	 = property.read(this);
			auto slotIndex	 = mo->indexOfSlot("uniformChanged()");
			auto signalIndex = property.notifySignalIndex();
			QMetaObject::connect(this, signalIndex, this, slotIndex);
			mUniforms[name] = QVariant();
			uniformChanged();
		}
	}
}

QQmlListProperty<RenderableObject> RenderableObject::renderables() const
{
	return QQmlListProperty<RenderableObject>(
		reinterpret_cast<QObject *>(const_cast<RenderableObject *>(this)),
		reinterpret_cast<void *>(const_cast<RenderableObject *>(this)),
		&RenderableObject::appendRenderable,
		&RenderableObject::countRenderables, &RenderableObject::renderable,
		&RenderableObject::clearRenderables,
		&RenderableObject::replaceRenderable,
		&RenderableObject::removeLastRenderable);
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

void RenderableObject::clearRenderables(
	QQmlListProperty<RenderableObject> *renderableList)
{
	reinterpret_cast<RenderableObject *>(renderableList->data)
		->clearRenderables();
}

int RenderableObject::countRenderables(
	QQmlListProperty<RenderableObject> *renderableList)
{
	return reinterpret_cast<RenderableObject *>(renderableList->data)
		->countRenderables();
}

RenderableObject *
RenderableObject::renderable(QQmlListProperty<RenderableObject> *renderableList,
							 int pos)
{
	return reinterpret_cast<RenderableObject *>(renderableList->data)
		->renderable(pos);
}

void RenderableObject::replaceRenderable(
	QQmlListProperty<RenderableObject> *renderableList, int pos,
	RenderableObject *renderable)
{
	return reinterpret_cast<RenderableObject *>(renderableList->data)
		->replaceRenderable(pos, renderable);
}
void RenderableObject::removeLastRenderable(
	QQmlListProperty<RenderableObject> *renderableList)
{
	reinterpret_cast<RenderableObject *>(renderableList->data)
		->removeLastRenderable();
}

} // namespace Graphics
} // namespace qmlmodule
