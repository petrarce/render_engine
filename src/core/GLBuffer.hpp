#pragma once
#include <glad/glad.h>

#include <GLDefinitions.hpp>
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <vector>
namespace glwrapper
{
namespace core
{
template <GLenum BufferType, GLenum BufferBindingType>
class GLBuffer : public GLObject
{
public:
	enum ImmutableStorageType {
		Read = GL_MAP_READ_BIT,
		Write = GL_MAP_WRITE_BIT,
		Dynamic = GL_DYNAMIC_STORAGE_BIT,
		Persistent = GL_MAP_PERSISTENT_BIT,
		Coherent = GL_MAP_COHERENT_BIT,
		ClientStorage = GL_CLIENT_STORAGE_BIT,
	};

	enum MutableStorageType {
		StaticDraw = GL_STATIC_DRAW,
		StaticCopy = GL_STATIC_COPY,
		StaticRead = GL_STATIC_READ,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicCopy = GL_DYNAMIC_COPY,
		DynamicRead = GL_DYNAMIC_READ,
		StreamDraw = GL_STREAM_DRAW,
		StreamCopy = GL_STREAM_COPY,
		StreamRead = GL_STREAM_READ,
	};

	GLBuffer(const std::string &name = "GLBuffer")
		: GLObject(name)
	{
		glGenBuffers(1, &mObjectId);
	}

	~GLBuffer() override
	{
		glDeleteBuffers(1, &mObjectId);
	}

	/// Create mutable storage for the buffer of specified type and deploy it
	/// with data in buff
	template <class T>
	void create(const std::vector<T> &buff, GLenum type)
	{
		GlObjectBinder bind(*this);
		glBufferData(BufferType, buff.size() * sizeof(T), buff.data(), type);
	}

	/// create mutable storage for buffer without storing data there
	void create(GLuint size, GLenum type)
	{
		GlObjectBinder bind(*this);
		glBufferData(BufferType, size, nullptr, type);
	}

	/// Create immutable storage for the buffer
	/** After creation buffer size should not be changed
	 * thus all operations are performed on the same location
	 */
	template <class T>
	void createImmutable(const std::vector<T> &buff, GLenum type)
	{
		GlObjectBinder bind(*this);
		glBufferStorage(BufferType, buff.size() * sizeof(T), buff.data(), type);
	}

	/// Create immutable storage without initialisation
	void createImmutable(size_t size, GLenum type)
	{
		GlObjectBinder bind(*this);
		glBufferStorage(BufferType, size, nullptr, type);
	}

protected:
	void bind() override
	{
		glGetIntegerv(BufferBindingType, &mObjectBeforeBinding);
		glBindBuffer(BufferType, mObjectId);
	}
	void unbind() override
	{
		glBindBuffer(BufferType, mObjectBeforeBinding);
	}
};

using GLArrayBuffer = GLBuffer<GL_ARRAY_BUFFER, GL_ARRAY_BUFFER_BINDING>;
using GLElementArrayBuffer =
	GLBuffer<GL_ELEMENT_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER_BINDING>;

} // namespace core
} // namespace glwrapper
