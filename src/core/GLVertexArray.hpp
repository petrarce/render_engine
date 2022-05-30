#pragma once
#include <GLBuffer.hpp>
#include <GLDefinitions.hpp>
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <iostream>

namespace dream
{
namespace glwrapper
{
class GLVertexArray : public GLObject
{
public:
	struct AttributeSpecification
	{
		GLint components{ 1 };
		GLint instanceLevel{ 0 };
		GLuint location{ 0 };
		bool normalize;
		size_t offset;
		GLsizei stride;
		GLenum type;
	};
	GLVertexArray(const std::string &name = "GLVertexArray")
		: GLObject(name)
	{
		glGenVertexArrays(1, &mObjectId);
	}

	~GLVertexArray()
	{
		glDeleteVertexArrays(1, &mObjectId);
	}

	template <class Buffer, class... Buffers>
	void createAttribute(const AttributeSpecification &spec, Buffer &buffer,
						 Buffers &...args)
	{
		GLObjectBinder binder(buffer);
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
		GLObjectBinder binder(*this);

		// now bind ElementArrayBuffer that was intended for VertexArray
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eob);

		glVertexAttribPointer(spec.location, spec.components, spec.type,
							  spec.normalize, spec.stride,
							  reinterpret_cast<const void *>(spec.offset));
		glEnableVertexAttribArray(spec.location);
		glVertexBindingDivisor(spec.location, spec.instanceLevel);
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

} // namespace glwrapper
} // namespace dream
