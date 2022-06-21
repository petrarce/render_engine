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
					   glwrapper::GLProgram &program) const = 0;
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
			   glwrapper::GLProgram &program) const final
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
	const T &value() const
	{
		return mValue;
	}
	T &value()
	{
		return mValue;
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
			   glwrapper::GLProgram &program) const final
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
			   glwrapper::GLProgram &program) const override
	{
	}
};

template <class ValueT>
class Option : public Variable
{
public:
	Option(const ValueT &value)
		: Variable()
		, mValue(value)
	{
	}
	~Option()
	{
	}
	void apply(const std::string &name,
			   glwrapper::GLProgram &program) const override
	{
	}
	const ValueT &value() const
	{
		return mValue;
	}
	void setValue(const ValueT &value)
	{
		mValue = value;
	}

private:
	ValueT mValue;
};

} // namespace components
} // namespace dream
