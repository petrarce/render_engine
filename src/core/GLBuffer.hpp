#pragma once
#include <glad/glad.h>

#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <vector>
namespace glwrapper
{
namespace core
{
class GLBuffer : public GLObject
{
public:
	GLBuffer(const std::string &name = "GLBuffer");
	~GLBuffer() override;

	template <class T> void create(std::vector<T> &buff, GLenum type)
	{
		GlObjectBinder bind(*this);
		glBufferData(this->type(), buff.size() * sizeof(T), buff.data(), type);
	}

	void create(GLuint size, GLenum type)
	{
		GlObjectBinder bind(*this);
		glBufferData(this->type(), size, nullptr, type);
	}

protected:
	virtual GLenum type() = 0;
};
} // namespace core
} // namespace glwrapper
