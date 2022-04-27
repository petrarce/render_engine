#pragma once
#include <iostream>
#include <GLObject.hpp>
#include <GLBuffer.hpp>
#include <GLObjectBinder.hpp>

namespace glwrapper {

namespace core {

class GLVertexArray : public GLObject
{
public:
	struct AttributeSpecification {
		GLuint location {0};
		GLint components {1};
		GLenum type;
		bool normalize;
		GLsizei stride;
		const void *offset;
	};
	GLVertexArray(const std::string& name = "GLVertexArray")
		: GLObject(name)
	{
		glGenVertexArrays(1, &mObjectId);
	}
	~GLVertexArray()
	{
		glDeleteVertexArrays(1, &mObjectId);
	}
	
	template<class Buffer, class... Buffers>
	void createAttribute(const AttributeSpecification& spec, 
						 Buffer& buffer, 
						 Buffers& ...args)
	{
		GlObjectBinder binder(buffer);
		createAttribute(spec, args...);
	}
protected:
	void createAttribute(const AttributeSpecification& spec){
		GlObjectBinder binder(*this);
		glVertexAttribPointer(spec.location, 
							  spec.components,
							  spec.type,
							  spec.normalize,
							  spec.stride,
							  spec.offset);
		glEnableVertexAttribArray(spec.location);
	}

	void bind() override
	{
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &mObjectBeforeBinding);
		glBindVertexArray(mObjectId);
	}
	void unbind() override
	{
		glBindVertexArray(mObjectBeforeBinding);
	}
};

}
}