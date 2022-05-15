#include "GLObjectBinder.hpp"

#include <iostream>
namespace dream
{
namespace glwrapper
{
GlObjectBinder::GlObjectBinder(GLObject &object)
	: mGlObject(object)
{
	mGlObject.bind();
}

GlObjectBinder::~GlObjectBinder()
{
	mGlObject.unbind();
}

} // namespace glwrapper
} // namespace dream
