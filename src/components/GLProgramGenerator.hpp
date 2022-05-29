#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <molecular/programgenerator/ProgramFile.h>
#include <molecular/programgenerator/ProgramGenerator.h>
#include "Singleton.hpp"
namespace dream
{
namespace components
{
class GLProgramGenerator
	: public molecular::programgenerator::ProgramGenerator
	, public Singleton<GLProgramGenerator>
{
public:
	friend class Singleton<GLProgramGenerator>;

	void deploy(std::string program)
	{

		char *str = const_cast<char *>(program.c_str());
		molecular::programgenerator::ProgramFile file(str,
													  &str[program.size() - 1]);

		const auto &variables = file.GetVariables();
		for (const auto &var : variables) AddVariable(var);

		const auto &functions = file.GetFunctions();
		for (const auto &func : functions) AddFunction(func);
	}

	void deployFile(std::string file)
	{
		std::ifstream inpFile(file, std::ios_base::in);
		if (!inpFile.is_open())
			throw std::runtime_error("Cannot open file for reading: " + file);
		std::stringstream ss;
		ss << inpFile.rdbuf();
		deploy(ss.str());
	}

private:
	GLProgramGenerator()
		: molecular::programgenerator::ProgramGenerator()
		, Singleton<GLProgramGenerator>()
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
};

} // namespace components
} // namespace dream
