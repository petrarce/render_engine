#pragma once
#include <GLRenderableObject.hpp>
#include <GLAssetManager.hpp>
#include <GLWrapperCore>
#include <GLComponents>
#include <molecular/util/Hash.h>
namespace dream
{
namespace renderer
{
class GLScreenTextureRenderFunction
	: public components::GLMultipleCaleeRenderFunction
{
public:
	GLScreenTextureRenderFunction()
		: components::GLMultipleCaleeRenderFunction()
	{
	}
	virtual ~GLScreenTextureRenderFunction()
	{
	}

	void setViewport(const std::array<int, 4> &viewport)
	{
		using namespace molecular::util;
		if (mViewport[2] != viewport[2] || mViewport[3] != viewport[3])
		{
			auto st = GLAssetManager<glwrapper::GLTexture2D>::getAsset(
				"ScreenTexture"_H);
			auto rb = GLAssetManager<glwrapper::GLRenderBuffer>::getAsset(
				"ScreenDepthBuffer"_H);

			std::array<std::size_t, 2> size;
			size[0] = viewport[2];
			size[1] = viewport[3];

			st->create<2>(
				0, dream::glwrapper::GLTexture2D::InternalFormat::Rgba, size);
			rb->create(size[0], size[1], GL_DEPTH24_STENCIL8);
		}
		mViewport = viewport;
	}

	const std::array<int, 4> &viewport() const
	{
		return mViewport;
	}
	void init() override
	{
		components::GLMultipleCaleeRenderFunction::init();

		using namespace molecular::util;
		auto fbo = GLAssetManager<glwrapper::GLFrameBufferObject>::addAsset(
			"ScreenFramebuffer"_H,
			std::make_shared<glwrapper::GLFrameBufferObject>());
		auto st = GLAssetManager<glwrapper::GLTexture2D>::addAsset(
			"ScreenTexture"_H, std::make_shared<glwrapper::GLTexture2D>());
		auto rb = GLAssetManager<glwrapper::GLRenderBuffer>::addAsset(
			"ScreenDepthBuffer"_H,
			std::make_shared<glwrapper::GLRenderBuffer>());
		st->setParameter(
			dream::glwrapper::GLTexture2D::ValueMinFilter::MinLinear);
		st->setParameter(
			dream::glwrapper::GLTexture2D::ValueMagFilter::MagLinear);
		std::array<std::size_t, 2> size;
		size[0] = mViewport[2];
		size[1] = mViewport[3];
		st->create<2>(0, dream::glwrapper::GLTexture2D::InternalFormat::Rgba,
					  size);
		rb->create(size[0], size[1], GL_DEPTH24_STENCIL8);
		dream::glwrapper::GLDrawFramebufferBinder bindFBO(*fbo);
		fbo->attach(
			dream::glwrapper::GLFrameBufferObject::AttachmentPoint::Color0,
			*st);
		fbo->attach(dream::glwrapper::GLFrameBufferObject::DepthStencil, *rb);

		mScreenRectangle.init();
	}

private:
	void drawImpl(const components::Scope &parentScope) override
	{
		using namespace molecular::util;
		glViewport(mViewport[0], mViewport[1], mViewport[2], mViewport[3]);
		components::Scope scope(parentScope);
		{
			auto fbo = GLAssetManager<glwrapper::GLFrameBufferObject>::getAsset(
				"ScreenFramebuffer"_H);
			glwrapper::GLDrawFramebufferBinder bindFBO(*fbo);
			glEnable(GL_DEPTH_TEST);
			glClearColor(0.2345f, 0.492f, 0.717f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if (bindFBO.state() !=
				dream::glwrapper::GLDrawFramebufferBinder::Complete)
				throw std::runtime_error(
					"Failed to initialize screen renderbuffer target error: " +
					std::to_string(bindFBO.state()));
			GLMultipleCaleeRenderFunction::drawImpl(scope);
		}
		mScreenRectangle.draw(scope);
	}

	class DrawScreenRectangle : public GLRenderFunction
	{
	public:
		DrawScreenRectangle()
		{
		}

		void init() override
		{
			GLRenderFunction::init();
			mVAB = std::make_unique<dream::glwrapper::GLArrayBuffer>();
			mEAB = std::make_unique<dream::glwrapper::GLElementArrayBuffer>();
			mVAO = std::make_unique<dream::glwrapper::GLVertexArray>();

			mVAB->create(
				std::vector<float>{
					-1, -1, 0,	0, 0,
					1, -1, 0,	1, 0,
					-1, 1, 0,	0, 1,
					1, 1, 0,	1, 1,
				},
				GL_STATIC_DRAW);
			mEAB->create(std::vector<unsigned int>{ 0, 1, 2, 2, 1, 3 },
						 GL_STATIC_DRAW);
			// vertex position attribute
			mVAO->createAttribute(
				dream::glwrapper::GLVertexArray::AttributeSpecification{
					.components	   = 3,
					.instanceLevel = 0,
					.location	   = 0,
					.normalize	   = false,
					.offset		   = 0,
					.stride		   = sizeof(float) * 5,
					.type		   = GL_FLOAT,
				},
				*mVAB, *mEAB);
			// texture coordinates attribute
			mVAO->createAttribute(
				dream::glwrapper::GLVertexArray::AttributeSpecification{
					.components	   = 2,
					.instanceLevel = 0,
					.location	   = 1,
					.normalize	   = false,
					.offset		   = sizeof(float) * 3,
					.stride		   = sizeof(float) * 5,
					.type		   = GL_FLOAT,
				},
				*mVAB, *mEAB);
		}

	protected:
		void drawImpl(const dream::components::Scope &scp) override
		{
			using namespace molecular::util;

			dream::components::Scope scope(scp);
			prepareScope(scope);
			auto screenTexture =
				GLAssetManager<glwrapper::GLTexture2D>::getAsset(
					"ScreenTexture"_H);

			screenTexture->attach(dream::glwrapper::GLTextureUnit::Texture0 +
								  1);

			auto program =
				GLAssetManager<components::GLMolecularProgram>::addAsset(scope);
			program->prepare(scope);

			glDisable(GL_DEPTH_TEST);
			glClearColor(1.0f, 1.0f, 1.0f,
						 1.0f); // set clear color to white (not really
								// necessary actually, since we won't be able to
								// see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT);

			dream::glwrapper::GLObjectBinder bindProg(*program);
			dream::glwrapper::GLObjectBinder binVAO(*mVAO);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}

		void prepareScope(components::Scope &scope) override
		{
			using namespace molecular::util;
			scope.Set("uScreenTexture"_H, dream::components::Uniform<int>(1));
			scope.Set("aVerPos"_H,
					  dream::components::Attribute<Eigen::Vector3f>());
			scope.Set("aTexCoord"_H,
					  dream::components::Attribute<Eigen::Vector2f>());
		}
		std::unique_ptr<dream::glwrapper::GLElementArrayBuffer> mEAB;
		std::unique_ptr<dream::glwrapper::GLArrayBuffer> mVAB;
		std::unique_ptr<dream::glwrapper::GLVertexArray> mVAO;
	};
	std::array<int, 4> mViewport{ 0, 0, 1, 1 };
	DrawScreenRectangle mScreenRectangle;
};
} // namespace renderer
} // namespace dream

namespace dream
{
namespace renderer
{
class GLScreenTextureRenderObject : public GLRenderableObject
{
public:
	GLScreenTextureRenderObject(
		const std::string &name = "GLScreenTextureRenderObject")
		: GLRenderableObject(name)
	{
		mRenderFunction = std::make_shared<GLScreenTextureRenderFunction>();
	}
	void syncSelf() override
	{
		auto rf = std::static_pointer_cast<GLScreenTextureRenderFunction>(
			mRenderFunction);
		if (mViewportChanged)
		{
			rf->setViewport(mViewport);
			mViewportChanged = false;
		}
	}

	void setViewport(const std::array<int, 4> &viewport)
	{
		mViewport		 = viewport;
		mViewportChanged = true;
	}

	const std::array<int, 4> viewport() const
	{
		return mViewport;
	}

private:
	std::array<int, 4> mViewport;
	bool mViewportChanged{ true };
};
} // namespace renderer
} // namespace dream
