#pragma once
#include <QQuickItem>
#include <GLRenderer>
#include <QQmlListProperty>
#include <QQmlParserStatus>
#include <QList>
#include <src/ui/qt/helpers/EigenConversions.hpp>

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
public Q_SLOTS:
	void uniformChanged()
	{
		auto mo = this->metaObject();
		for (int i = 0; i < mo->propertyCount(); i++)
		{
			auto property = mo->property(i);
			QString name(property.name());
			QVariant value = property.read(this);
			if (mUniforms.contains(name) && value != mUniforms.value(name))
			{
				mUniforms[name] = value;
				Q_EMIT uniformChanged(name.mid(sizeof("u_") - 1), value);
			}
		}
	}

Q_SIGNALS:
	void update();
	void uniformChanged(const QString &name, const QVariant &value);

protected:
	/// Uniforms handler.
	/**
	 * this defines how are uniforms handled by the subclass
	 * returns true if uniform was accepted
	 */
	virtual bool handleUniform(const QString &name, const QVariant &value)
	{
		if (!value.isValid())
			return false;
		QMetaType::Type type = static_cast<QMetaType::Type>(value.type());

		if (type == QMetaType::Int || type == QMetaType::Bool)
		{
			mRenderableObject->setUniform(name.toStdString(),
										  value.value<int>());
			return true;
		}
		else if (type == QMetaType::Float || type == QMetaType::Double)
		{
			mRenderableObject->setUniform(name.toStdString(),
										  value.value<float>());
			return true;
		}
		else if (type == QMetaType::QVector2D)
		{
			mRenderableObject->setUniform(
				name.toStdString(),
				qt::helpers::toEigen(value.value<QVector2D>()));
			return true;
		}
		else if (type == QMetaType::QVector3D)
		{
			mRenderableObject->setUniform(
				name.toStdString(),
				qt::helpers::toEigen(value.value<QVector3D>()));
			return true;
		}
		else if (type == QMetaType::QVector4D)
		{
			mRenderableObject->setUniform(
				name.toStdString(),
				qt::helpers::toEigen(value.value<QVector4D>()));
			return true;
		}
		else if (type == QMetaType::QMatrix)
		{
			mRenderableObject->setUniform(
				name.toStdString(),
				qt::helpers::toEigen(value.value<QMatrix4x4>()));
			return true;
		}
		else if (type == QMetaType::QMatrix4x4)
		{
			mRenderableObject->setUniform(
				name.toStdString(),
				qt::helpers::toEigen(value.value<QMatrix4x4>()));
			return true;
		}
		else if (type == QMetaType::QColor)
		{
			auto color = value.value<QColor>();
			mRenderableObject->setUniform(
				name.toStdString(),
				Eigen::Vector4f(color.red() / 255.f, color.green() / 255.f,
								color.blue() / 255.f, color.alpha() / 255.f));
			return true;
		}

		return false;
	}

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
