#include "GLRenderFunction.hpp"

namespace dream
{
namespace components
{

GLSingleCaleeRenderFunction::GLSingleCaleeRenderFunction(
	std::shared_ptr<GLRenderFunction> calee)
	: GLRenderFunction()
	, mCalee(calee)
{
}
void GLSingleCaleeRenderFunction::drawImpl(const Scope &parentScope)
{
	mCalee->draw(parentScope);
}
void GLSingleCaleeRenderFunction::setCalee(
	std::shared_ptr<GLRenderFunction> calee)
{
	mCalee = calee;
}

GLMultipleCaleeRenderFunction::GLMultipleCaleeRenderFunction(
	const CaleeArray &calees)
	: GLRenderFunction()
	, mCalees(calees)
{
}

void GLMultipleCaleeRenderFunction::setCalees(const CaleeArray &calees)
{
	mCalees = calees;
}

void GLMultipleCaleeRenderFunction::drawImpl(const Scope &parentScope)
{
	for (const auto &c : mCalees) c->draw(parentScope);
}

} // namespace components
} // namespace dream
