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
	bool isComplete();

	GLint getId() const
	{
		return mObjectId;
	}

private:
	void bind() override;
	void unbind() override;

	GLint mDrawFBBinding{ 0 };
	GLint mReadFBBinding{ 0 };
	GLint mReadDrawFBBinding{ 0 };
};

template <GLenum BindingType>
class GLFramebufferBinder : public GLObjectBinder
{
public:
	GLFramebufferBinder(GLFrameBufferObject &fbo)
		: GLObjectBinder(fbo)
	{
		glBindFramebuffer(BindingType, fbo.getId());
	}
};

using GLReadFramebufferBinder	  = GLFramebufferBinder<GL_READ_FRAMEBUFFER>;
using GLDrawFramebufferBinder	  = GLFramebufferBinder<GL_DRAW_FRAMEBUFFER>;
using GLReadDrawFramebufferBinder = GLFramebufferBinder<GL_FRAMEBUFFER>;

} // namespace glwrapper
} // namespace dream