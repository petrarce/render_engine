#pragma once
#include <glad/glad.h>
#include <GLBuffer.hpp>
namespace glwrapper {

namespace core {

class GLArrayBuffer : public GLBuffer
{
public:
	GLArrayBuffer(const std::string& name = "GLArrayBuffer") : GLBuffer(name) {};
	~GLArrayBuffer() override {};
	
protected:
	void bind() override
	{
		glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &mObjectBeforeBinding);
		glBindBuffer(GL_ARRAY_BUFFER, mObjectId);
	}
	void unbind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, mObjectBeforeBinding);
	}
	
	GLenum type() override { return GL_ARRAY_BUFFER; }

};
}
}
