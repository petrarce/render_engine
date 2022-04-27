#pragma once
#include <Shader.hpp>

class Renderable
{
public:
    virtual void setup(Shader& shader) const = 0;
};
