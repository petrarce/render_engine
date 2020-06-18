#include <molecular/programgenerator/ProgramGenerator.h>
#include <molecular/programgenerator/ProgramFile.h>
#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <sstream>
#include <molecular/util/Hash.h>

#define prdbg(msg, args...) fprintf(stderr, "DBG: " msg "\n", ##args)
using namespace std;
using namespace molecular::programgenerator;
using namespace molecular::util;

int main(int argc, char** argv)
{
    ProgramGenerator generator;
    for(int i = 1; i < argc; i++)
    {
        std::ifstream file(argv[i]);
        assert(file.is_open() == true);
        stringstream ss;
        ss << file.rdbuf();
        prdbg("file %d:\n%s\n", i, ss.str().c_str());
        char* content = new char[ss.str().size()];
        memcpy(content, ss.str().c_str(), ss.str().size());
        ProgramFile prfile(content, content + ss.str().size() - 1);
        for(const auto& var : prfile.GetVariables())
            generator.AddVariable(var);
        for(const auto& funct : prfile.GetFunctions())
            generator.AddFunction(funct);
    }
    std::set<Hash> inp = {"textureCoordAttribute"_H,
                            "diffuseTexture"_H,
                            //"somevar"_H,
                         };
    std::set<Hash> out = {"fragmentColor"_H};
    ProgramGenerator::ProgramText prText = generator.GenerateProgram(inp, out);
    prdbg("Final vertex shader:\n %s\n", prText.vertexShader.c_str());
    prdbg("Final fragment shader:\n %s\n", prText.fragmentShader.c_str());
    return 0;
}
