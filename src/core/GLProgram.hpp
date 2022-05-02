#pragma once
#include <glad/glad.h>

#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <GLShader.hpp>

namespace glwrapper
{
namespace core
{
class GLProgram : public GLObject
{
public:
	GLProgram(const std::string &name = "");
	~GLProgram() override;

	/// Link shaders. All shaders shoud be compiled before linkage
	template <class Shader, class... Shaders>
	bool link(Shader &shader, Shaders &...shaders)
	{
		glAttachShader(mObjectId, shader.mObjectId);
		return link(shaders...);
	}

	bool linkStatus()
	{
		int success;
		glGetProgramiv(mObjectId, GL_LINK_STATUS, &success);
		return success;
	}
	std::string linkageLog()
	{
		if (!linkStatus()) {
			std::string log(65542, 0);
			int finalSize = 0;
			glGetProgramInfoLog(
				mObjectId, log.size(), &finalSize,
				reinterpret_cast<GLchar *>(const_cast<char *>(log.data())));
			log.resize(finalSize);
			return log;
		}
		return "Success";
	}

#define UNIFORM_SETTER(typeSuffix, type) \
	template<class... VarueArgs> \
	void setUniform(const std::string& var, type v1, const VarueArgs&... val) \
	{ \
		int components = sizeof...(val) + 1; \
		GLint location = glGetUniformLocation(mObjectId, var.c_str()); \
	 \
		if(components == 1) \
			glUniform1##typeSuffix(location, v1, val...); \
		else if(components == 2) \
			glUniform1##typeSuffix(location, v1, val...); \
		else if(components == 3) \
			glUniform1##typeSuffix(location, v1, val...); \
		else if(components == 4) \
			glUniform1##typeSuffix(location, v1, val...); \
		else \
			throw std::runtime_error("Failed to set uniform with more than 4 elements"); \
	}


	UNIFORM_SETTER(f, float)
	UNIFORM_SETTER(i, int)
	UNIFORM_SETTER(ui, unsigned int)

	void use() { glUseProgram(mObjectId); }

protected:
	bool link()
	{
		glLinkProgram(mObjectId);
		return linkStatus();
	}

	void bind() override;
	virtual void unbind() override;
};

} // namespace core
} // namespace glwrapper
