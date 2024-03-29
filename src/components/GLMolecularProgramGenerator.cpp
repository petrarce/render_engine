#include "GLMolecularProgramGenerator.hpp"

namespace dream
{
namespace components
{

void GLMolecularProgramGenerator::deploy(std::string program)
{
	char *str = const_cast<char *>(program.c_str());
	molecular::programgenerator::ProgramFile file(str,
												  &str[program.size() - 1]);

	const auto &variables = file.GetVariables();
	for (const auto &var : variables) AddVariable(var);

	const auto &functions = file.GetFunctions();
	for (const auto &func : functions) AddFunction(func);
}

void GLMolecularProgramGenerator::deployFile(std::string file)
{
	std::ifstream inpFile(file, std::ios_base::in);
	if (!inpFile.is_open())
		throw std::runtime_error("Cannot open file for reading: " + file);
	std::stringstream ss;
	ss << inpFile.rdbuf();
	deploy(ss.str());
}

GLMolecularProgramGenerator::GLMolecularProgramGenerator()
	: molecular::programgenerator::ProgramGenerator()
	, Singleton<GLMolecularProgramGenerator>()
{
	try
	{
		deployFile("../assets/DefaultGlslShader.glslgen");
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << "../assets/DefaultGlslShader.glslgen";
	}
}

} // namespace components
} // namespace dream
