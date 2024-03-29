#pragma once
#include <QQuickFramebufferObject>
#include <QQmlListProperty>
#include <GLRenderer>
#include <GLComponents>
#include <QStringList>
#include <QDebug>
#include "RenderableObject.hpp"
#include "Camera.hpp"

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
	Q_PROPERTY(Camera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
	Q_PROPERTY(OrbitCamera *defaultCamera READ defaultCamera CONSTANT)
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
	Camera *camera() const
	{
		return mCamera;
	}
	OrbitCamera *defaultCamera() const
	{
		return mDefaultCamera;
	}

	void setRootRenderableObject(RenderableObject *rootRenderableObject);
	void setMode(RenderingMode mode);
	void setCamera(Camera *camera);

Q_SIGNALS:
	void rootRenderableObjectChanged(RenderableObject *rootRenderableObject);
	void modeChanged(RenderingMode mode);
	void cameraChanged(const Camera *camera);
	void viewTransformChanged(const QMatrix4x4 &view);

private:
	void mousePressEvent(QMouseEvent *event) override
	{
		mLastMousePress = event->pos();
		mPressedButton	= event->button();
		event->accept();
	}

	void mouseReleaseEvent(QMouseEvent *event) override
	{
		event->accept();
	}

	void mouseMoveEvent(QMouseEvent *event) override
	{
		auto offset		= mLastMousePress - event->pos();
		mLastMousePress = event->pos();
		if (mPressedButton == Qt::MouseButton::LeftButton)
		{
			mCamera->rotateCamera(offset.x() / 500.f, -offset.y() / 500.f);
			event->accept();
		}
		else if (mPressedButton == Qt::MouseButton::RightButton)
		{
			mCamera->moveCamera(offset.x() / 100.f, offset.y() / 100.f);
			event->accept();
		}
		else if (mPressedButton == Qt::MouseButton::MiddleButton)
		{
			mCamera->zoomCamera(offset.y() / 10.f);
			event->accept();
		}
		else
		{
			event->ignore();
			return;
		}
	}

	void wheelEvent(QWheelEvent *event) override
	{
		event->accept();
	}

	RenderableObject *mRootRenderableObject{ nullptr };
	QStringList mRootScope;
	RenderingMode mMode;
	QPoint mLastMousePress;
	Qt::MouseButton mPressedButton;
	OrbitCamera *mDefaultCamera;
	Camera *mCamera;
};

} // namespace Graphics
} // namespace qmlmodule
