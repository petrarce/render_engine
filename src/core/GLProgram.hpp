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
