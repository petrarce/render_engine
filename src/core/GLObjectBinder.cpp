#include "GLObjectBinder.hpp"

#include <iostream>
namespace dream
{
namespace glwrapper
{
GLObjectBinder::GLObjectBinder(GLObject &object)
	: mGlObject(object)
{
	mGlObject.bind();
}

GLObjectBinder::~GLObjectBinder()
{
	mGlObject.unbind();
}

} // namespace glwrapper
} // namespace dream
