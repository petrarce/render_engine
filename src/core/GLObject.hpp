#pragma once
#include <glad/glad.h>

#include <string>
#include <vector>
namespace dream
{
namespace glwrapper
{
class GLObjectBinder;

class GLObject
{
public:
	GLObject(const std::string &name = "")
		: mName(name)
	{
		mObjectsBeforeBinding.reserve(10);
	};
	virtual ~GLObject() = default;

	// make GL opject noncopyable and nonmovable
	GLObject(const GLObject &)					= delete;
	GLObject(GLObject &&)						= delete;
	const GLObject &operator=(const GLObject &) = delete;
	GLObject &operator=(GLObject &&)			= delete;

	friend class GLObjectBinder;

	std::string mName{ "GLObject" };

	GLuint getId() const
	{
		return mObjectId;
	}

protected:
	virtual void bind()	  = 0;
	virtual void unbind() = 0;

	GLuint mObjectId{ 0 };
	std::vector<GLint> mObjectsBeforeBinding;
};

} // namespace glwrapper
} // namespace dream
