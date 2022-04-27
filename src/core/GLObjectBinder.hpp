#pragma once
#include <GLObject.hpp>

namespace glwrapper {
namespace core {
class GlObjectBinder
{
public: 
	GlObjectBinder(GLObject& object);
	~GlObjectBinder();
private:
	GLObject& mGlObject;
};

}
}