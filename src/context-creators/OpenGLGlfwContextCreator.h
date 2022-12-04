#ifndef OPENGLGLFWCONTEXTCREATOR_H
#define OPENGLGLFWCONTEXTCREATOR_H
#include "GLGlfwContextCreator.hpp"
namespace dream
{

namespace glcontext
{

class OpenGLGlfwContextCreator : public GlfwContextCreator
{
public:
	OpenGLGlfwContextCreator(int width = 1, int height = 1,
							 const std::string &name = "GLGlfwContextCreator");

private:
	void createContext() final;
};

} // namespace glcontext

} // namespace dream
#endif // OPENGLGLFWCONTEXTCREATOR_H
