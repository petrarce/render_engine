#include "GLFrameBufferObject.hpp"
#include "GLTexture.hpp"
namespace dream
{
namespace glwrapper
{

void GLFrameBufferObject::bind()
{
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &mReadFBBinding);
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &mDrawFBBinding);
}

void GLFrameBufferObject::unbind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mDrawFBBinding);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mReadFBBinding);
}

void GLFrameBufferObject::attach(AttachmentPoint attachment,
								 GLTexture2D &texture, int mipmapLevel)
{
	GLObjectBinder bindTexture(texture);
	GLDrawFramebufferBinder bindFBO(*this);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, attachment, GL_TEXTURE_2D,
						   texture.getId(), mipmapLevel);
}

void GLFrameBufferObject::attach(AttachmentPoint attachment,
								 GLRenderBuffer &renderBuffer)
{
	GLObjectBinder bindRBO(renderBuffer);
	GLDrawFramebufferBinder bindFBO(*this);
}

bool GLFrameBufferObject::isComplete()
{
	GLReadDrawFramebufferBinder bind(*this);
	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

} // namespace glwrapper
} // namespace dream