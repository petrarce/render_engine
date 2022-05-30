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
class GLMolecularProgramGenerator
	: public molecular::programgenerator::ProgramGenerator
	, public Singleton<GLMolecularProgramGenerator>
{
public:
	friend class Singleton<GLMolecularProgramGenerator>;

	void deploy(std::string program);

	void deployFile(std::string file);

private:
	GLMolecularProgramGenerator();
};

} // namespace components
} // namespace dream
