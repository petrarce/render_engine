#pragma once
#include <GLWrapperCore>
#include <molecular/gfx/Scope.h>
#include <molecular/programgenerator/ProgramGenerator.h>
#include "Variable.hpp"

namespace dream
{
namespace components
{

using Scope = molecular::util::Scope<Variable>;

class GLProgramContainer : public glwrapper::GLShaderProgram
{
public:
	GLProgramContainer()
		: glwrapper::GLShaderProgram()
	{
	}

	void generateProgram(const Scope &scp);
	void prepareProgram(const Scope &scp);

protected:
	size_t mProgramHash = 0;
	std::map<molecular::util::Hash, std::string> mUniforms;
};

} // namespace components
} // namespace dream
