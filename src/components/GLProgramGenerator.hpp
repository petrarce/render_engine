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

	void deploy(std::string program);

	void deployFile(std::string file);

private:
	GLProgramGenerator();
};

} // namespace components
} // namespace dream
