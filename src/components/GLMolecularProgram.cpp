#include <functional>
#include <molecular/programgenerator/ProgramGenerator.h>
#include <molecular/util/Hash.h>
#include <GLWrapperCore>
#include "GLMolecularProgramGenerator.hpp"
#include "GLMolecularProgram.hpp"
#include "Variable.hpp"

using namespace molecular::programgenerator;

namespace std
{

template <>
struct hash<std::set<molecular::util::Hash>>
{
	size_t operator()(const std::set<molecular::util::Hash> &vset)
	{
		size_t result = 0;
		for (auto v : vset) result ^= std::hash<molecular::util::Hash>{}(v);
		return result;
	}
};

} // namespace std

namespace dream
{
namespace components
{

void GLMolecularProgram::generate(const Scope &scp)
{
	std::set<molecular::util::Hash> variables;
	auto scomeMap = scp.ToMap();
	for (auto v : scomeMap) variables.insert(v.first);
	size_t scopeHash = std::hash<std::set<molecular::util::Hash>>{}(variables);
	if (scopeHash == mProgramHash)
		return;
	mProgramHash = scopeHash;

	ProgramGenerator &generator = GLMolecularProgramGenerator::instance();
	ProgramGenerator::ProgramText text =
		generator.GenerateProgram(variables.begin(), variables.end());
	text.vertexShader =
		glwrapper::GLProgram::getWokingGlslVersionString() + text.vertexShader;
	text.fragmentShader = glwrapper::GLProgram::getWokingGlslVersionString() +
						  text.fragmentShader;
	try
	{
		glwrapper::GLProgram::prepare(text.vertexShader, text.fragmentShader);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
	}

	mUniforms.clear();
	auto uniforms = getActiveUniforms();
	for (const auto &u : uniforms)
		mUniforms.insert({ molecular::util::HashUtils::MakeHash(u), u });
}

void GLMolecularProgram::prepare(const Scope &scp)
{
	glwrapper::GLObjectBinder bind(*this);
	for (const auto &u : mUniforms)
	{
		if (scp.Has(u.first))
		{
			const Variable &var = scp.Get<Variable>(u.first);
			var.apply(u.second, *this);
		}
	}
}

} // namespace components
} // namespace dream
