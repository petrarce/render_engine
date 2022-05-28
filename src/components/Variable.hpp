#pragma once
#include <GLWrapperCore>
#include <GLProgramObject.hpp>

namespace dream
{
namespace components
{
class Variable
{
public:
	Variable()
	{
	}
	virtual ~Variable()
	{
	}
	virtual void apply(const std::string &name,
					   glwrapper::GLShaderProgram &program) const = 0;
};

template <class T>
class Uniform : public Variable
{
public:
	Uniform(const T &value)
		: Variable()
		, mValue(value)
	{
	}
	virtual ~Uniform(){};
	void apply(const std::string &name,
			   glwrapper::GLShaderProgram &program) const final
	{
		program.setUniform(name, mValue);
	}

private:
	T mValue;
};

template <class T>
class Attribute : public Variable
{
public:
	Attribute()
		: Variable()
	{
	}
	virtual ~Attribute()
	{
	}

	void apply(const std::string &name,
			   glwrapper::GLShaderProgram &program) const final
	{
		// Do nothing
	}
};

class Output : public Variable
{
public:
	Output()
		: Variable()
	{
	}
	~Output()
	{
	}
	void apply(const std::string &name,
			   glwrapper::GLShaderProgram &program) const override
	{
	}
};

} // namespace components
} // namespace dream
