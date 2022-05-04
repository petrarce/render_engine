#ifndef GLDEFINITIONS_HPP
#define GLDEFINITIONS_HPP

#include <iostream>
#include <sstream>
#include <glad/glad.h>

#ifdef GL_ENABLE_THROW_ON_ERROR
#define GL_THROW_ON_ERROR() gl_throw_on_error()
#else
#define GL_THROW_ON_ERROR() do {} while(0) //noop
#endif

inline void gl_throw_on_error()
{
	try
	{
		if(int err = glGetError() != GL_NO_ERROR)
		{
			std::stringstream ss;
			ss << std::hex << err;
			throw std::runtime_error("opengl call failed with error " + ss.str());
		}
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(e.what());
	}
}

#endif // GLDEFINITIONS_HPP
