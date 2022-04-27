#include "GLObjectBinder.hpp"
#include <iostream>
namespace glwrapper {
namespace core {

GlObjectBinder::GlObjectBinder(GLObject& object)
	: mGlObject(object)
{
	std::cerr << "Bind " << mGlObject.mName << std::endl;
	mGlObject.bind();
}

GlObjectBinder::~GlObjectBinder()
{
	mGlObject.unbind();
	std::cerr << "Unbind " << mGlObject.mName << std::endl;
	
}

}
}