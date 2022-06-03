#pragma once
#include <GLWrapperCore>
#include <molecular/gfx/Scope.h>
#include <molecular/programgenerator/ProgramGenerator.h>
#include "Variable.hpp"

namespace dream
{
namespace components
{

typedef molecular::util::Scope<dream::components::Variable> Scope;

class GLMolecularProgram : public glwrapper::GLProgram
{
public:
	GLMolecularProgram()
		: glwrapper::GLProgram()
	{
	}

	void generate(const Scope &scp);
	void prepare(const Scope &scp);

protected:
	size_t mProgramHash = 0;
	std::map<molecular::util::Hash, std::string> mUniforms;
};

} // namespace components
} // namespace dream