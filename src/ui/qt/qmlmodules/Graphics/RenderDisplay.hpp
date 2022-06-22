#pragma once
#include <QQuickFramebufferObject>
#include <QQmlListProperty>
#include <GLRenderer>
#include "RenderableObject.hpp"

namespace qmlmodule
{
namespace Graphics
{
class RenderDisplay : public QQuickFramebufferObject
{
	Q_OBJECT
	Q_PROPERTY(
		RenderableObject *rootRenderableObject READ rootRenderableObject WRITE
			setRootRenderableObject NOTIFY rootRenderableObjectChanged)
	Q_CLASSINFO("DefaultProperty", "rootRenderableObject")

public:
	class Renderer : public QQuickFramebufferObject::Renderer
	{
	public:
		Renderer(const RenderDisplay &renderDisplay);
		virtual ~Renderer();

		void render() override;
		void synchronize(QQuickFramebufferObject *) override;

	private:
		const RenderDisplay &mRenderDisplay;
	};

	RenderDisplay(QQuickItem *parent = nullptr);
	~RenderDisplay();
	QQuickFramebufferObject::Renderer *createRenderer() const override;

	RenderableObject *rootRenderableObject() const
	{
		return mRootRenderableObject;
	}

	void setRootRenderableObject(RenderableObject *rootRenderableObject)
	{
		if (mRootRenderableObject == rootRenderableObject)
			return;

		mRootRenderableObject = rootRenderableObject;
		connect(mRootRenderableObject, &RenderableObject::update, this,
				&QQuickFramebufferObject::update);
		Q_EMIT rootRenderableObjectChanged(mRootRenderableObject);
	}

Q_SIGNALS:
	void rootRenderableObjectChanged(RenderableObject *rootRenderableObject);

private:
	RenderableObject *mRootRenderableObject{ nullptr };
};

} // namespace Graphics
} // namespace qmlmodule
