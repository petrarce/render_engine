#ifndef GLDEFINITIONS_HPP
#define GLDEFINITIONS_HPP

#include <glad/glad.h>
#include <iostream>
#include <map>
#include <sstream>

volatile extern int init_gl_callbacks_var;
static const volatile int duplicate_init_gl_callbacks_var =
	init_gl_callbacks_var;
std::string glEnumString(GLenum enumVal);

#endif // GLDEFINITIONS_HPP
