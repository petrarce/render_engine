#pragma once
#include <glad/glad.h>

#include <GLDefinitions.hpp>
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

	void use()
	{
		glUseProgram(mObjectId);
	}

#define UNIFORM_SETTER(tp, type, dim)                                          \
	template <class... VarueArgs>                                              \
	void setUniform##dim(const std::string &var, type v1,                      \
						 const VarueArgs &...val)                              \
	{                                                                          \
		GLint location = glGetUniformLocation(mObjectId, var.c_str());         \
		glUniform##dim##tp(location, v1, val...);                              \
	}

#define UNIFORM_SETTER_API(tp, type)                                           \
	UNIFORM_SETTER(tp, type, 1)                                                \
	UNIFORM_SETTER(tp, type, 2)                                                \
	UNIFORM_SETTER(tp, type, 3)                                                \
	UNIFORM_SETTER(tp, type, 4)

	UNIFORM_SETTER_API(f, float)
	UNIFORM_SETTER_API(i, int)
	UNIFORM_SETTER_API(ui, unsigned int)

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
