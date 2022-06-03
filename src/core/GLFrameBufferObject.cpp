#include "GLFrameBufferObject.hpp"
#include "GLTexture.hpp"
namespace dream
{
namespace glwrapper
{

void GLFrameBufferObject::bind()
{
	mDrawReadFBsBeforeBinding.push_back({ 0, 0 });
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING,
				  &mDrawReadFBsBeforeBinding.back().read);
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING,
				  &mDrawReadFBsBeforeBinding.back().draw);
}

void GLFrameBufferObject::unbind()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,
					  mDrawReadFBsBeforeBinding.back().draw);
	glBindFramebuffer(GL_READ_FRAMEBUFFER,
					  mDrawReadFBsBeforeBinding.back().read);
	mDrawReadFBsBeforeBinding.pop_back();
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
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, attachment, GL_RENDERBUFFER,
							  renderBuffer.getId());
}

} // namespace glwrapper
} // namespace dream
