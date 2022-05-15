#pragma once
#include <glad/glad.h>

#include <GLDefinitions.hpp>
#include <GLObject.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
namespace dream
{
namespace glwrapper
{
class GLProgram;

template <GLuint ShaderType>
class GLShader : public GLObject
{
public:
	GLShader(const std::string &name = "")
		: GLObject(name)
	{
		mObjectId = glCreateShader(ShaderType);
	};
	virtual ~GLShader()
	{
		glDeleteShader(mObjectId);
	}

	bool compile(const std::string &source)
	{
		int size			  = source.size();
		const GLchar *dataPtr = reinterpret_cast<const GLchar *>(source.data());
		glShaderSource(mObjectId, 1, &dataPtr, &size);
		glCompileShader(mObjectId);

		return compileStatus();
	}

	bool compileFile(const std::string &filePath)
	{
		std::ifstream file(filePath, std::ios_base::in);
		if (!file.is_open())
			throw std::runtime_error("Failed to open shader file: " + filePath);

		std::stringstream ss;
		ss << file.rdbuf();
		std::string fileContent = ss.str();
		compile(fileContent);

		return compileStatus();
	}

	bool compileStatus()
	{
		int success;
		glGetShaderiv(mObjectId, GL_COMPILE_STATUS, &success);
		return success;
	}

	std::string compilationLog()
	{
		if (compileStatus() == GL_FALSE)
		{
			std::string compileLog;
			int logSize = 0;
			compileLog.resize(65536, 0);
			glGetShaderInfoLog(mObjectId, compileLog.size(), &logSize,
							   reinterpret_cast<GLchar *>(
								   const_cast<char *>(compileLog.data())));
			compileLog.resize(logSize);
			return compileLog;
		}
		return "Succeed";
	}

protected:
	friend class GLProgram;
	void bind() override
	{
	}
	void unbind() override
	{
		// returning attachment is not relevant
		// TODO: add attachment of old shader if necessary
	}
};

using GLVertexShader   = GLShader<GL_VERTEX_SHADER>;
using GLFragmentShader = GLShader<GL_FRAGMENT_SHADER>;
using GLGeometryShader = GLShader<GL_GEOMETRY_SHADER>;
using GLComputeShader  = GLShader<GL_COMPUTE_SHADER>;

} // namespace glwrapper
} // namespace dream
