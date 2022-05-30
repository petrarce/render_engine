#pragma once
#include <glad/glad.h>

#include <GLDefinitions.hpp>
#include <GLObject.hpp>
#include <GLObjectBinder.hpp>
#include <GLShader.hpp>
#include <Eigen/Dense>

#include <vector>
#include <utility>
#include <set>

namespace dream
{
namespace glwrapper
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
		if (!linkStatus())
		{
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

	template <int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
	void setUniform(const std::string &name,
					const Eigen::Matrix<float, _Rows, _Cols, _Options, _MaxRows,
										_MaxCols> &mat,
					bool transpose = false)
	{
		typedef void (*GlMatrixSetter)(GLint location, GLsizei count,
									   GLboolean transpose,
									   const GLfloat *value);
		if (mat.cols() > 4 || mat.rows() > 4 || mat.rows() < 2 ||
			mat.cols() < 2)
			throw std::runtime_error("invalid matrix size");

		static std::map<std::pair<int, int>, GlMatrixSetter> functions = {
			{ std::make_pair(2, 2), glUniformMatrix2fv },
			{ std::make_pair(2, 3), glUniformMatrix2x3fv },
			{ std::make_pair(2, 4), glUniformMatrix2x4fv },

			{ std::make_pair(3, 2), glUniformMatrix3x2fv },
			{ std::make_pair(3, 3), glUniformMatrix3fv },
			{ std::make_pair(3, 4), glUniformMatrix3x4fv },

			{ std::make_pair(4, 2), glUniformMatrix4x2fv },
			{ std::make_pair(4, 3), glUniformMatrix4x3fv },
			{ std::make_pair(4, 4), glUniformMatrix4fv },
		};
		GLint location = glGetUniformLocation(mObjectId, name.c_str());
		functions.find(std::make_pair(mat.rows(), mat.cols()))
			->second(location, 1, transpose, mat.data());
	}

	std::set<std::string> getActiveUniforms()
	{
		GLint uniforms;
		glGetProgramiv(mObjectId, GL_ACTIVE_UNIFORMS, &uniforms);
		std::set<std::string> uniformNames;
		for (int i = 0; i < uniforms; i++)
		{
			char name[256];
			GLsizei length;
			GLsizei size;
			GLenum type;
			glGetActiveUniform(mObjectId, i, 256, &length, &size, &type, name);
			uniformNames.insert(std::string(name, &name[length]));
		}
		return uniformNames;
	}

	void setUniform(const std::string &name, int val);
	void setUniform(const std::string &name, float val);
	void setUniform(const std::string &name, unsigned int val);
	void setUniform(const std::string &name, std::array<int, 2> val);
	void setUniform(const std::string &name, std::array<float, 2> val);
	void setUniform(const std::string &name, std::array<unsigned int, 2> val);
	void setUniform(const std::string &name, std::array<int, 3> val);
	void setUniform(const std::string &name, std::array<float, 3> val);
	void setUniform(const std::string &name, std::array<unsigned int, 3> val);
	void setUniform(const std::string &name, std::array<int, 4> val);
	void setUniform(const std::string &name, std::array<float, 4> val);
	void setUniform(const std::string &name, std::array<unsigned int, 4> val);

	template <class ArgT, class... Args,
			  // only for integers and floats
			  std::enable_if_t<std::is_integral<ArgT>::value ||
								   std::is_floating_point<ArgT>::value,
							   bool> = true>
	void setUniform(const std::string &name, ArgT val1, const Args... vals)
	{
		setUniform(name,
				   std::array<ArgT, sizeof...(vals) + 1>({ val1, vals... }));
	}

	void prepare(const std::string &vertexShaderText,
				 const std::string &fragmentShaderText);

	void prepareFiles(const std::string &vertexShaderPath,
					  const std::string &fragmentShaderPath);

	static std::string getWokingGlslVersionString();

protected:
	bool link()
	{
		glLinkProgram(mObjectId);
		return linkStatus();
	}

	void bind() override;
	virtual void unbind() override;
};

} // namespace glwrapper
} // namespace dream
