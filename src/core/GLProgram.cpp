#include <GLProgram.hpp>
namespace glwrapper
{
namespace core
{
GLProgram::GLProgram(const std::string &name)
	: GLObject(name)
{
	mObjectId = glCreateProgram();
	GL_THROW_ON_ERROR();
};
GLProgram::~GLProgram() { glDeleteProgram(mObjectId); }

void GLProgram::bind()
{
	glGetIntegerv(GL_ACTIVE_PROGRAM, &mObjectBeforeBinding);
	GL_THROW_ON_ERROR();
	glUseProgram(mObjectId);
}

void GLProgram::unbind() {
	glUseProgram(mObjectBeforeBinding);
	GL_THROW_ON_ERROR();
}

} // namespace core
} // namespace glwrapper
