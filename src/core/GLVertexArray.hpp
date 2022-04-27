#pragma once
#include <iostream>
#include <GLObject.hpp>
#include <GLBuffer.hpp>
#include <GLElementArrayBuffer.hpp>
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
	void createAttribute(const AttributeSpecification& spec, GLElementArrayBuffer& ebo)
	{
		GlObjectBinder bindEbo(ebo);
		GlObjectBinder bindVA(*this);

		//since VertexArray internally binds ElementArrayBuffer that was active before VertexArra was
		// unbind need to rebind again ElementArrayBuffer so that this buffer bound previously
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo.mObjectId);

		glVertexAttribPointer(spec.location,
							  spec.components,
							  spec.type,
							  spec.normalize,
							  spec.stride,
							  spec.offset);
		glEnableVertexAttribArray(spec.location);


	}
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
