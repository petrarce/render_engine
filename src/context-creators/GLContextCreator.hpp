#pragma once
#include <string>
namespace dream
{
namespace glcontext
{
class GLContextCreator
{
public:
	friend class GLContextBinder;

	GLContextCreator(const std::string &name = "GLContextCreator")
	{
	}
	virtual ~GLContextCreator()
	{
	}
	GLContextCreator(const GLContextCreator &)			  = delete;
	GLContextCreator &operator=(const GLContextCreator &) = delete;
	GLContextCreator(GLContextCreator &&)				  = delete;
	GLContextCreator &operator=(GLContextCreator &&)	  = delete;

	std::string mName;

protected:
	virtual void createContext()  = 0;
	virtual void destroyContext() = 0;

private:
};

class GLContextBinder
{
public:
	GLContextBinder(GLContextCreator &context)
		: mContextCreator(context)
	{
		mContextCreator.createContext();
	}
	~GLContextBinder()
	{
		mContextCreator.destroyContext();
	}

	GLContextBinder()									= delete;
	GLContextBinder(const GLContextBinder &)			= delete;
	GLContextBinder &operator=(const GLContextBinder &) = delete;
	GLContextBinder(GLContextBinder &&)					= delete;
	GLContextBinder &operator=(GLContextBinder &&)		= delete;

private:
	GLContextCreator &mContextCreator;
};

} // namespace glcontext
} // namespace dream
