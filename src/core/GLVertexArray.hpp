#pragma once
#include <GLBuffer.hpp>
#include <GLDefinitions.hpp>
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <iostream>

namespace glwrapper
{
namespace core
{
class GLVertexArray : public GLObject
{
public:
	struct AttributeSpecification {
		GLint components{1};
		GLuint location{0};
		bool normalize;
		size_t offset;
		GLsizei stride;
		GLenum type;
	};
	GLVertexArray(const std::string &name = "GLVertexArray")
		: GLObject(name)
	{
		glGenVertexArrays(1, &mObjectId);
		GL_THROW_ON_ERROR();
	}

	~GLVertexArray()
	{
		glDeleteVertexArrays(1, &mObjectId);
		GL_THROW_ON_ERROR();
	}

	template <class Buffer, class... Buffers>
	void createAttribute(const AttributeSpecification &spec, Buffer &buffer,
						 Buffers &...args)
	{
		GlObjectBinder binder(buffer);
		createAttribute(spec, args...);
	}

protected:
	void createAttribute(const AttributeSpecification &spec)
	{
		// read off current binding of ElementArrayBuffer since VertexArrey
		// during bind internally will overwrite ElementArrayBinding with value
		// of ElementArrayObject which is refferenced by VertexArray
		GLint eob;
		glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eob);
		GL_THROW_ON_ERROR();
		GlObjectBinder binder(*this);

		// now bind ElementArrayBuffer that was intended for VertexArray
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob);
		GL_THROW_ON_ERROR();

		glVertexAttribPointer(spec.location, spec.components, spec.type,
							  spec.normalize, spec.stride,
							  reinterpret_cast<const void *>(spec.offset));
		GL_THROW_ON_ERROR();
		glEnableVertexAttribArray(spec.location);
		GL_THROW_ON_ERROR();
	}

	void bind() override
	{
		glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &mObjectBeforeBinding);
		GL_THROW_ON_ERROR();
		glBindVertexArray(mObjectId);
		GL_THROW_ON_ERROR();
	}
	void unbind() override
	{
		glBindVertexArray(mObjectBeforeBinding);
		GL_THROW_ON_ERROR();
	}
};

} // namespace core
} // namespace glwrapper
