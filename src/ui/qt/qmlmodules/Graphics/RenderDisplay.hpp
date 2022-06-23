#pragma once
#include <QQuickFramebufferObject>
#include <QQmlListProperty>
#include <GLRenderer>
#include <GLComponents>
#include <QStringList>
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
	Q_PROPERTY(QStringList rootScope READ rootScope WRITE setRootScope NOTIFY
				   rootScopeChanged)
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
		dream::components::Scope mRenderingRootScope;
	};

	RenderDisplay(QQuickItem *parent = nullptr);
	~RenderDisplay();
	QQuickFramebufferObject::Renderer *createRenderer() const override;

	RenderableObject *rootRenderableObject() const { return mRootRenderableObject; }
	const QStringList& rootScope() const { return mRootScope; }

	void setRootRenderableObject(RenderableObject *rootRenderableObject);
	void setRootScope(const QStringList &rootScope);

Q_SIGNALS:
	void rootRenderableObjectChanged(RenderableObject *rootRenderableObject);
	void rootScopeChanged(const QStringList &rootScope);

private:
	RenderableObject *mRootRenderableObject{ nullptr };
	QStringList mRootScope;
};

} // namespace Graphics
} // namespace qmlmodule
