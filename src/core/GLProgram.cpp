#include <GLProgram.hpp>
namespace dream
{
namespace glwrapper
{
GLProgram::GLProgram(const std::string &name)
	: GLObject(name)
{
	mObjectId = glCreateProgram();
}
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

void GLProgram::setUniform(const std::string &name, int val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform1i(loc, val);
}

void GLProgram::setUniform(const std::string &name, float val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform1f(loc, val);
}

void GLProgram::setUniform(const std::string &name, unsigned int val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform1ui(loc, val);
}

void GLProgram::setUniform(const std::string &name, std::array<int, 2> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform2i(loc, val[0], val[1]);
}
void GLProgram::setUniform(const std::string &name, std::array<float, 2> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform2f(loc, val[0], val[1]);
}

void GLProgram::setUniform(const std::string &name,
						   std::array<unsigned int, 2> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform2ui(loc, val[0], val[1]);
}

void GLProgram::setUniform(const std::string &name, std::array<int, 3> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform3i(loc, val[0], val[1], val[2]);
}
void GLProgram::setUniform(const std::string &name, std::array<float, 3> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform3f(loc, val[0], val[1], val[2]);
}
void GLProgram::setUniform(const std::string &name,
						   std::array<unsigned int, 3> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform3ui(loc, val[0], val[1], val[2]);
}
void GLProgram::setUniform(const std::string &name, std::array<int, 4> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform4i(loc, val[0], val[1], val[2], val[3]);
}
void GLProgram::setUniform(const std::string &name, std::array<float, 4> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform4f(loc, val[0], val[1], val[2], val[3]);
}
void GLProgram::setUniform(const std::string &name,
						   std::array<unsigned int, 4> val)
{
	auto loc = glGetUniformLocation(mObjectId, name.c_str());
	glUniform4ui(loc, val[0], val[1], val[2], val[3]);
}

void GLProgram::prepare(const std::string &vertexShaderText,
						const std::string &fragmentShaderText)
{
	GLVertexShader vs;
	GLFragmentShader fs;
	if (!vs.compile(vertexShaderText))
		throw std::runtime_error("Vertex compilation failed:\n " +
								 vs.compilationLog());
	if (!fs.compile(fragmentShaderText))
		throw std::runtime_error("Fragment compilation failed:\n " +
								 fs.compilationLog());
	if (!link(vs, fs))
		throw std::runtime_error("Link error:\n" + linkageLog() +
								 "Vertex Shader:\n " + vertexShaderText +
								 "Fragment Shader:\n" + fragmentShaderText);
}

void GLProgram::prepareFiles(const std::string &vertexShaderPath,
							 const std::string &fragmentShaderPath)
{
	std::ofstream vsFile(vertexShaderPath, std::ios::in);
	std::ofstream fsFile(vertexShaderPath, std::ios::in);

	if (!vsFile.is_open() || !fsFile.is_open())
		throw std::runtime_error(
			std::string("Failed to open shader file: ") +
			(vsFile.is_open() ? vertexShaderPath : fragmentShaderPath));

	GLVertexShader vs;
	GLFragmentShader fs;
	std::stringstream vsStream;
	vsStream << vsFile.rdbuf();
	std::stringstream fsStream;
	fsStream << fsFile.rdbuf();

	prepare(vsStream.str(), fsStream.str());
}

std::string GLProgram::getWokingGlslVersionString()
{
	static std::string currentVersion = "";

	if (!currentVersion.empty())
		return currentVersion;
	std::string program				  = "void main() {"
										"	gl_Position = vec4(1,2,3,0.3);"
										"}";
	std::vector<std::string> versions = {
		std::string("#version 330\n"),	  std::string("#version 460\n"),
		std::string("#version 440\n"),	  std::string("#version 450\n"),
		std::string("#version 430\n"),	  std::string("#version 420\n"),
		std::string("#version 410\n"),	  std::string("#version 400\n"),
		std::string("#version 150\n"),	  std::string("#version 140\n"),
		std::string("#version 130\n"),	  std::string("#version 120\n"),
		std::string("#version 110\n"),	  std::string("#version 100\n"),
		std::string("#version 320 es\n"), std::string("#version 310 es\n"),
		std::string("#version 300 es\n"),
	};

	GLVertexShader vs;
	for (const auto &v : versions)
	{
		std::string newProgram = v + program;
		if (vs.compile(newProgram))
		{
			currentVersion = v;
			break;
		}
	}
	return currentVersion;
}

} // namespace glwrapper
} // namespace dream
