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

	void draw(const Scope &parentScope);

private:
	CaleeArray mCalees;
};
} // namespace components
} // namespace dream
