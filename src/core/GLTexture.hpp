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
template <GLenum TextureType, GLenum TextureBindingType>
class GLTexture : public GLObject
{
public:
	GLTexture(const std::string &name = "GLTexture")
		: GLObject(name)
	{
		glGenTextures(1, &mObjectId);
		GL_THROW_ON_ERROR();
	}

	~GLTexture() override
	{
		glDeleteTextures(1, &mObjectId);
		GL_THROW_ON_ERROR();
	}

	/// Create mutable storage for the buffer of specified type and deploy it
	/// with data in buff
	template <class T>
	void create(std::vector<T> &buff, GLenum type)
	{
	}

	/// create mutable storage for buffer without storing data there
	void create(GLuint size, GLenum type)
	{
	}

	/// Create immutable storage for the buffer
	/** After creation buffer size should not be changed
	 * thus all operations are performed on the same location
	 */
	template <class T>
	void createImmutable(std::vector<T> &buff, GLenum type)
	{
	}

	/// Create immutable storage without initialisation
	void createImmutable(size_t size, GLenum type)
	{
	}

protected:
	void bind() override
	{
		glGetIntegerv(TextureBindingType, &mObjectBeforeBinding);
		GL_THROW_ON_ERROR();
		glBindTexture(TextureType, mObjectId);
		GL_THROW_ON_ERROR();
	}

	void unbind() override
	{
		glBindTexture(TextureType, mObjectBeforeBinding);
		GL_THROW_ON_ERROR();
	}
};

using GLTexture2D = GLTexture<GL_TEXTURE_2D, GL_TEXTURE_BINDING_2D>;

} // namespace core
} // namespace glwrapper
