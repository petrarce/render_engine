#pragma once
#include <GLWrapperCore>

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
	virtual Variable &operator=(const Variable &other)
	{
		return *this;
	}
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
	Variable &operator=(const Variable &other) override
	{
		const Uniform<T> *otherUniform =
			reinterpret_cast<const Uniform<T> *>(&other);
		if (otherUniform)
			mValue = otherUniform->mValue;

		return *this;
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
