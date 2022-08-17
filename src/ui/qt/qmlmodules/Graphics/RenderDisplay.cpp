#include "RenderDisplay.hpp"
#include <glad/glad.h>
#include <mutex>
#include <QOpenGLFramebufferObject>
#include <molecular/util/Hash.h>
#include <MathHelpers.hpp>
namespace qmlmodule
{
namespace Graphics
{

RenderDisplay::Renderer::Renderer(const RenderDisplay &renderDisplay)
	: QQuickFramebufferObject::Renderer()
	, mRenderDisplay(renderDisplay)
{
	static bool init = false;
	static std::mutex m;
	std::lock_guard<std::mutex> lock(m);
	if (!init)
		if (!gladLoadGL())
			throw std::runtime_error("Failed to load glad");
}

RenderDisplay::Renderer::~Renderer()
{
}

void RenderDisplay::Renderer::render()
{
	glEnable(GL_FRAMEBUFFER_SRGB);
	glClearColor(dream::helpers::linearToGamma(94.3 / 255.f),
				 dream::helpers::linearToGamma(62.2 / 255.f),
				 dream::helpers::linearToGamma(10.1 / 255.f), 1);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	mRenderDisplay.rootRenderableObject()
		->renderableObject()
		->renderFunction()
		->draw(*mRenderingRootScope);
	glDisable(GL_FRAMEBUFFER_SRGB);
}

void RenderDisplay::Renderer::synchronize(QQuickFramebufferObject *)
{
	using namespace molecular::util;
	mRenderDisplay.rootRenderableObject()->renderableObject()->resetSync();
	mRenderDisplay.rootRenderableObject()->renderableObject()->sync();

	auto newRootScope = std::make_unique<dream::components::Scope>();
	for (const auto &v : mRenderDisplay.mRootScope)
		newRootScope->Set(molecular::util::HashUtils::MakeHash(v.toStdString()),
						  dream::components::Attribute<void>());
	mRenderingRootScope = std::move(newRootScope);
}

QOpenGLFramebufferObject *
RenderDisplay::Renderer::createFramebufferObject(const QSize &size)
{
	QOpenGLFramebufferObjectFormat format;
	format.setAttachment(QOpenGLFramebufferObject::Attachment::Depth);
	//	format.setInternalTextureFormat(GL_SRGB8_ALPHA8);
	//	format.setInternalTextureFormat(GL_RGBA);

	format.setSamples(4);

	return new QOpenGLFramebufferObject(size, format);
}

RenderDisplay::RenderDisplay(QQuickItem *parent)
	: QQuickFramebufferObject(parent)
	, mRootScope({ "fragColor", "gl_Position" })
	, mDefaultCamera(new OrbitCamera(this))
	, mCamera(mDefaultCamera)
{
	setTextureFollowsItemSize(true);
	setAcceptedMouseButtons(Qt::AllButtons);
	connect(mCamera, &Camera::viewMatrixChanged, this,
			&RenderDisplay::viewTransformChanged);
}

RenderDisplay::~RenderDisplay()
{
}

QQuickFramebufferObject::Renderer *RenderDisplay::createRenderer() const
{
	return new RenderDisplay::Renderer(*this);
}

void RenderDisplay::setRootRenderableObject(
	RenderableObject *rootRenderableObject)
{
	if (mRootRenderableObject == rootRenderableObject)
		return;

	if (mRootRenderableObject)
		disconnect(mRootRenderableObject, nullptr, this, nullptr);
	mRootRenderableObject = rootRenderableObject;
	connect(mRootRenderableObject, &RenderableObject::update, this,
			&QQuickFramebufferObject::update);
	Q_EMIT rootRenderableObjectChanged(mRootRenderableObject);
}

void RenderDisplay::setMode(RenderingMode mode)
{
	if (mMode == mode)
		return;

	mMode = mode;
	Q_EMIT modeChanged(mMode);
}

void RenderDisplay::setCamera(Camera *camera)
{
	if (mCamera == camera)
		return;

	disconnect(mCamera, nullptr, this, nullptr);
	mCamera = camera;
	connect(mCamera, &Camera::viewMatrixChanged, this,
			&RenderDisplay::viewTransformChanged);
}

} // namespace Graphics
} // namespace qmlmodule
