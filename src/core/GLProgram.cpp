#include <GLProgram.hpp>
namespace glwrapper
{
namespace core
{
GLProgram::GLProgram(const std::string &name)
	: GLObject(name)
{
	mObjectId = glCreateProgram();
};
GLProgram::~GLProgram() { glDeleteProgram(mObjectId); }

void GLProgram::bind()
{
	glGetIntegerv(GL_ACTIVE_PROGRAM, &mObjectBeforeBinding);
	glUseProgram(mObjectId);
}

void GLProgram::unbind() { glUseProgram(mObjectBeforeBinding); }

} // namespace core
} // namespace glwrapper