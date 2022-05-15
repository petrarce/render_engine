#pragma once
#include <glad/glad.h>

#include <string>
namespace dream
{
namespace glwrapper
{
class GlObjectBinder;

class GLObject
{
public:
	GLObject(const std::string &name = "")
		: mName(name){};
	virtual ~GLObject() = default;

	// make GL opject noncopyable and nonmovable
	GLObject(const GLObject &)					= delete;
	GLObject(GLObject &&)						= delete;
	const GLObject &operator=(const GLObject &) = delete;
	GLObject &operator=(GLObject &&)			= delete;

	friend class GlObjectBinder;

	std::string mName{ "GLObject" };

protected:
	virtual void bind()	  = 0;
	virtual void unbind() = 0;

	GLuint mObjectId{ 0 };
	GLint mObjectBeforeBinding{ 0 };
};

} // namespace glwrapper
} // namespace dream