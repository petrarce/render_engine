#pragma once
#include <glad/glad.h>

#include <GLDefinitions.hpp>
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <GLShader.hpp>
#include <Eigen/Dense>

#include <vector>
#include <utility>

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

	template <int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
	void setMatrix(
		const std::string &name,
		Eigen::Matrix<float, _Rows, _Cols, _Options, _MaxRows, _MaxCols> &mat,
		bool transpose = false)
	{
		typedef void (*GlMatrixSetter)(GLint location, GLsizei count,
									   GLboolean transpose,
									   const GLfloat *value);
		if (mat.cols() > 4 || mat.rows() > 4 || mat.rows() < 2 ||
			mat.cols() < 2)
			throw std::runtime_error("invalid matrix size");

		static std::map<std::pair<int, int>, GlMatrixSetter> functions = {
			{std::make_pair(2, 2), glUniformMatrix2fv},
			{std::make_pair(2, 3), glUniformMatrix2x3fv},
			{std::make_pair(2, 4), glUniformMatrix2x4fv},

			{std::make_pair(3, 2), glUniformMatrix3x2fv},
			{std::make_pair(3, 3), glUniformMatrix3fv},
			{std::make_pair(3, 4), glUniformMatrix3x4fv},

			{std::make_pair(4, 2), glUniformMatrix4x2fv},
			{std::make_pair(4, 3), glUniformMatrix4x3fv},
			{std::make_pair(4, 4), glUniformMatrix4fv},
		};
		GLint location = glGetUniformLocation(mObjectId, name.c_str());
		functions.find(std::make_pair(mat.rows(), mat.cols()))
			->second(location, 1, transpose, mat.data());
	}

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
