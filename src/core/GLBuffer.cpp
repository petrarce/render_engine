#include <GLBuffer.hpp>
#include <cassert>

namespace glwrapper {

namespace core {


GLBuffer::GLBuffer(const std::string& name)
	: GLObject(name)
{
	glGenBuffers(1, &mObjectId);
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &mObjectId);
}

}
}
