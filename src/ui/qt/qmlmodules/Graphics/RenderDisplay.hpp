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
	Q_PROPERTY(RenderingMode mode READ mode WRITE setMode NOTIFY modeChanged)
	Q_CLASSINFO("DefaultProperty", "rootRenderableObject")

public:
	enum RenderingMode
	{
		Defualt,
		Normals,
		Tangents,
		Bitangents
	};
	Q_ENUM(RenderingMode)
	class Renderer : public QQuickFramebufferObject::Renderer
	{
	public:
		Renderer(const RenderDisplay &renderDisplay);
		virtual ~Renderer();

		void render() override;
		void synchronize(QQuickFramebufferObject *) override;
		QOpenGLFramebufferObject *
		createFramebufferObject(const QSize &size) override;

	private:
		const RenderDisplay &mRenderDisplay;
		std::unique_ptr<dream::components::Scope> mRenderingRootScope;
	};

	friend class Renderer;
	RenderDisplay(QQuickItem *parent = nullptr);
	~RenderDisplay();
	QQuickFramebufferObject::Renderer *createRenderer() const override;

	RenderableObject *rootRenderableObject() const
	{
		return mRootRenderableObject;
	}
	RenderingMode mode() const
	{
		return mMode;
	}

	void setRootRenderableObject(RenderableObject *rootRenderableObject);
	void setMode(RenderingMode mode);
Q_SIGNALS:
	void rootRenderableObjectChanged(RenderableObject *rootRenderableObject);
	void modeChanged(RenderingMode mode);

private:
	RenderableObject *mRootRenderableObject{ nullptr };
	QStringList mRootScope;
	RenderingMode mMode;
};

} // namespace Graphics
} // namespace qmlmodule
