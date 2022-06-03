#pragma once
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <GLTexture.hpp>
#include <GLRenderBuffer.hpp>
namespace dream
{
namespace glwrapper
{

class GLFrameBufferObject : public GLObject
{
public:
	enum AttachmentPoint
	{
		Color0		 = GL_COLOR_ATTACHMENT0,
		Depth		 = GL_DEPTH_ATTACHMENT,
		Stencil		 = GL_STENCIL_ATTACHMENT,
		DepthStencil = GL_DEPTH_STENCIL_ATTACHMENT,
	};
	friend class GLObjectBinder;
	GLFrameBufferObject(const std::string &name = "GLFrameBufferObject")
		: GLObject(name)
	{
		glGenFramebuffers(1, &mObjectId);
	}
	~GLFrameBufferObject()
	{
		glDeleteFramebuffers(1, &mObjectId);
	}

	void attach(AttachmentPoint attachment, GLTexture2D &texture,
				int mipmapLevel = 0);
	void attach(AttachmentPoint attachment, GLRenderBuffer &renderBuffer);
	GLint getId() const
	{
		return mObjectId;
	}

private:
	void bind() override;
	void unbind() override;
	struct FramebufferBindings
	{
		GLint draw;
		GLint read;
	};

	std::vector<FramebufferBindings> mDrawReadFBsBeforeBinding;
};

template <GLenum BindingType>
class GLFramebufferBinder : public GLObjectBinder
{
public:
	enum FramebufferState
	{
		Complete			 = GL_FRAMEBUFFER_COMPLETE,
		Undefined			 = GL_FRAMEBUFFER_UNDEFINED,
		IncompleteAttachment = GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT,
		IncompleteMissingAttachment =
			GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT,
		IncompleteDrawBuffer   = GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER,
		IncompleteReadBuffer   = GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER,
		Insupporte			   = GL_FRAMEBUFFER_UNSUPPORTED,
		IncompleteMultisample  = GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE,
		IncompleteLayerTargets = GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS,
	};

	GLFramebufferBinder(GLFrameBufferObject &fbo)
		: GLObjectBinder(fbo)
	{
		glBindFramebuffer(BindingType, fbo.getId());
	}

	FramebufferState state()
	{
		return static_cast<FramebufferState>(
			glCheckFramebufferStatus(BindingType));
	}
};

using GLReadFramebufferBinder	  = GLFramebufferBinder<GL_READ_FRAMEBUFFER>;
using GLDrawFramebufferBinder	  = GLFramebufferBinder<GL_DRAW_FRAMEBUFFER>;
using GLReadDrawFramebufferBinder = GLFramebufferBinder<GL_FRAMEBUFFER>;

} // namespace glwrapper
} // namespace dream
