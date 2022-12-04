#pragma once
#include <string>
namespace dream
{
namespace glcontext
{
class ContextCreator
{
public:
	friend class ContextBinder;

	ContextCreator(const std::string &name = "GLContextCreator")
	{
	}
	virtual ~ContextCreator()
	{
	}
	ContextCreator(const ContextCreator &) = delete;
	ContextCreator &operator=(const ContextCreator &) = delete;
	ContextCreator(ContextCreator &&)				  = delete;
	ContextCreator &operator=(ContextCreator &&) = delete;

	std::string mName;

protected:
	virtual void createContext()  = 0;
	virtual void destroyContext() = 0;

private:
};

class ContextBinder
{
public:
	ContextBinder(ContextCreator &context)
		: mContextCreator(context)
	{
		mContextCreator.createContext();
	}
	~ContextBinder()
	{
		mContextCreator.destroyContext();
	}

	ContextBinder()						 = delete;
	ContextBinder(const ContextBinder &) = delete;
	ContextBinder &operator=(const ContextBinder &) = delete;
	ContextBinder(ContextBinder &&)					= delete;
	ContextBinder &operator=(ContextBinder &&) = delete;

private:
	ContextCreator &mContextCreator;
};

} // namespace glcontext
} // namespace dream
