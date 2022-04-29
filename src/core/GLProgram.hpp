#pragma once
#include <glad/glad.h>
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <GLShader.hpp>

namespace glwrapper {

namespace core {

class GLProgram : public GLObject{
public:
	GLProgram(const std::string& name = "");
	~GLProgram() override;
	
	/// Link shaders. All shaders shoud be compiled before linkage
	template<GLuint ShaderType, class... Shaders>
	void link(GLShader<ShaderType>& shader, Shaders&... shaders)
	{
		GlObjectBinder bind(shader);
		link(shaders...);
	}
	
	void use()
	{
		glUseProgram(mObjectId);
	}
	
protected:
	void link()
	{
		glLinkProgram(mObjectId);
	}
	
	void bind() override;
	virtual void unbind() override;

};

}
}