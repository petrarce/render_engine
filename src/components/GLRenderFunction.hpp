#pragma once
#include "GLProgramObject.hpp"

namespace dream
{
namespace components
{

class GLRenderFunction
{
public:
	virtual void draw(const Scope &parentScope) = 0;
};

class GLSingleCaleeRenderFunction : public GLRenderFunction
{
public:
	GLSingleCaleeRenderFunction(
		std::shared_ptr<GLRenderFunction> calee = nullptr)
		: GLRenderFunction()
		, mCalee(calee)
	{
	}
	void draw(const Scope &parentScope) override
	{
		mCalee->draw(parentScope);
	}
	void setCalee(std::shared_ptr<GLRenderFunction> calee)
	{
		mCalee = calee;
	}

private:
	std::shared_ptr<GLRenderFunction> mCalee;
};

class GLMultipleCaleeRenderFunction : public GLRenderFunction
{
public:
	using CaleePtr	 = std::shared_ptr<GLRenderFunction>;
	using CaleeArray = std::vector<CaleePtr>;
	GLMultipleCaleeRenderFunction(const CaleeArray &calees)
		: GLRenderFunction()
		, mCalees(calees)
	{
	}

	void setCalees(const CaleeArray &calees)
	{
		mCalees = calees;
	}

	void draw(const Scope &parentScope)
	{
		for (const auto &c : mCalees) c->draw(parentScope);
	}

private:
	CaleeArray mCalees;
};
} // namespace components
} // namespace dream
