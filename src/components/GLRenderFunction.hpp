#pragma once
#include "GLMolecularProgram.hpp"

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
		std::shared_ptr<GLRenderFunction> calee = nullptr);
	void draw(const Scope &parentScope) override;
	void setCalee(std::shared_ptr<GLRenderFunction> calee);

private:
	std::shared_ptr<GLRenderFunction> mCalee;
};

class GLMultipleCaleeRenderFunction : public GLRenderFunction
{
public:
	using CaleePtr	 = std::shared_ptr<GLRenderFunction>;
	using CaleeArray = std::vector<CaleePtr>;
	GLMultipleCaleeRenderFunction(const CaleeArray &calees);

	void setCalees(const CaleeArray &calees);
	void addCalee(CaleePtr calee)
	{
		mCalees.push_back(calee);
	}
	void removeCalee(CaleePtr calee)
	{
		std::remove_if(mCalees.begin(), mCalees.end(),
					   [calee](CaleePtr item) { return item == calee; });
	}

	void draw(const Scope &parentScope);

protected:
	CaleeArray mCalees;
};
} // namespace components
} // namespace dream
