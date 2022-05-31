#pragma once
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <GLTexture.hpp>

namespace dream
{
namespace glwrapper
{
class GLRenderBuffer : public GLObject
{
public:
	GLRenderBuffer(const std::string &name = "GLRenderBuffer")
		: GLObject(name)
	{
		glGenRenderbuffers(1, &mObjectId);
	}
	~GLRenderBuffer()
	{
		glDeleteRenderbuffers(1, &mObjectId);
	}

	void create(int width, int height, GLenum internalFromat, int samples = 1)
	{
		GLObjectBinder bind(*this);
		if (samples > 1)
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples,
											 internalFromat, width, height);
		else
			glRenderbufferStorage(GL_RENDERBUFFER, internalFromat, width,
								  height);
	}

private:
	void bind() override
	{
		glGetIntegerv(GL_RENDERBUFFER_BINDING, &mObjectBeforeBinding);
		glBindRenderbuffer(GL_RENDERBUFFER, mObjectId);
	}

	void unbind() override
	{
		glBindRenderbuffer(GL_RENDERBUFFER, mObjectBeforeBinding);
	}
};
} // namespace glwrapper
} // namespace dream