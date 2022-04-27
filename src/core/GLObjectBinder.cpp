#include "GLObjectBinder.hpp"
#include <iostream>
namespace glwrapper {
namespace core {

GlObjectBinder::GlObjectBinder(GLObject& object)
	: mGlObject(object)
{
	mGlObject.bind();
}

GlObjectBinder::~GlObjectBinder()
{
	mGlObject.unbind();
	
}

}
}
