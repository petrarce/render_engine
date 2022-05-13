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
GLProgram::~GLProgram()
{
	glDeleteProgram(mObjectId);
}

void GLProgram::bind()
{
	glGetIntegerv(GL_CURRENT_PROGRAM, &mObjectBeforeBinding);
	glUseProgram(mObjectId);
}

void GLProgram::unbind()
{
	glUseProgram(mObjectBeforeBinding);
}

void GLShaderProgram::prepare(const std::string &vertexShaderText,
							  const std::string &fragmentShaderText)
{
	GLVertexShader vs;
	GLFragmentShader fs;
	if (!vs.compile(vertexShaderText))
		throw std::runtime_error("Vertex compilation failed:\n " +
								 vs.compilationLog());
	if (!fs.compile(fragmentShaderText))
		throw std::runtime_error("Fragment compilation failed:\n " +
								 vs.compilationLog());
	if (!link(vs, fs))
		throw std::runtime_error("Link error:\n" + linkageLog());
}

void GLShaderProgram::prepareFiles(const std::string &vertexShaderPath,
								   const std::string &fragmentShaderPath)
{
	GLVertexShader vs;
	GLFragmentShader fs;
	if (!vs.compileFile(vertexShaderPath))
		throw std::runtime_error("Vertex compilation failed:\n " +
								 vs.compilationLog());
	if (!fs.compileFile(fragmentShaderPath))
		throw std::runtime_error("Fragment compilation failed:\n " +
								 vs.compilationLog());
	if (!link(vs, fs))
		throw std::runtime_error("Link error:\n" + linkageLog());
}

} // namespace core
} // namespace glwrapper
