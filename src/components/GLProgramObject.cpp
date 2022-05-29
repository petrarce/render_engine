#include <functional>
#include <molecular/programgenerator/ProgramGenerator.h>
#include <molecular/util/Hash.h>
#include <GLWrapperCore>
#include "GLProgramGenerator.hpp"
#include "GLProgramObject.hpp"
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

void GLProgramContainer::generateProgram(const Scope &scp)
{
	std::set<molecular::util::Hash> variables;
	auto scomeMap = scp.ToMap();
	for (auto v : scomeMap) variables.insert(v.first);
	size_t scopeHash = std::hash<std::set<molecular::util::Hash>>{}(variables);
	if (scopeHash == mProgramHash)
		return;
	mProgramHash = scopeHash;

	ProgramGenerator &generator = GLProgramGenerator::instance();
	ProgramGenerator::ProgramText text =
		generator.GenerateProgram(variables.begin(), variables.end());
	text.vertexShader =
		glwrapper::GLShaderProgram::getWokingGlslVersionString() +
		text.vertexShader;
	text.fragmentShader =
		glwrapper::GLShaderProgram::getWokingGlslVersionString() +
		text.fragmentShader;
	try
	{
		prepare(text.vertexShader, text.fragmentShader);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		std::cerr << "Vertex Shader:\n" << text.vertexShader << std::endl;
		std::cerr << "Fragment Shader:\n" << text.fragmentShader << std::endl;
	}

	mUniforms.clear();
	auto uniforms = getActiveUniforms();
	for (const auto &u : uniforms)
		mUniforms.insert({ molecular::util::HashUtils::MakeHash(u), u });
}

void GLProgramContainer::prepareProgram(const Scope &scp)
{
	glwrapper::GlObjectBinder bind(*this);
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
