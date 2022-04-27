#pragma once
#include <glad/glad.h>
#include <GLBuffer.hpp>
namespace glwrapper {

namespace core {

class GLElementArrayBuffer : public GLBuffer
{
public:
	GLElementArrayBuffer(const std::string& name = "GLElementArrayBuffer") : GLBuffer(name) {};
	~GLElementArrayBuffer() override {};
	
protected:
	void bind() override
	{
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &mObjectBeforeBinding);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObjectId);
	}
	void unbind() override
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mObjectBeforeBinding);
	}
	
	GLenum type() override { return GL_ELEMENT_ARRAY_BUFFER; }

};
}
}
