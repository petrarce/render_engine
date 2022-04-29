#pragma once
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <GLObject.hpp>
namespace glwrapper {

namespace core {

template<GLuint ShaderType>
class GLShader : public GLObject
{
public:
	GLShader(const std::string& name = "")
		:GLObject(name) 
	{
		mObjectId = glCreateShader(ShaderType);
	};
	virtual ~GLShader() 
	{
		glDeleteShader(mObjectId);
	}
	
	bool compile(const std::string& source)
	{
		int size = source.size();
		glShaderSource(mObjectId, 
			1, 
			reinterpret_cast<const GLchar* const*>(source.data()), 
			&size);
		glCompileShader(mObjectId);

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
		if(compileStatus() == GL_FALSE)
		{
			std::string compileLog;
			compileLog.resize(65536);
			glGetShaderInfoLog(mObjectId, 
							   compileLog.size(), 
							   nullptr,
							   compileLog.data());
			return compileLog;
		}
		return "Succeed";
	}
	
protected:
	void bind() override
	{
		glAttachShader(ShaderType, mObjectId);
	}
	void unbind() override
	{
		//returning attachment is not relevant
		//TODO: add attachment of old shader if necessary
	}

};

using GLVertexShader = GLShader<GL_VERTEX_SHADER>;
using GLFragmentShader = GLShader<GL_FRAGMENT_SHADER>;
using GLGeometryShader= GLShader<GL_GEOMETRY_SHADER>;
using GLComputeShader= GLShader<GL_COMPUTE_SHADER>;


}
}