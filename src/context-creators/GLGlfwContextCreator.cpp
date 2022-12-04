#include "GLGlfwContextCreator.hpp"
#include <string>
#include <iostream>

namespace dream
{

namespace glcontext
{
GlfwContextCreator::GlfwContextCreator(int width, int height,
									   const std::string &name)
	: ContextCreator(name)
	, mWindowWidth(width)
	, mWindowHeight(height)
{
	glfwInit();
}
GlfwContextCreator::~GlfwContextCreator()
{
	glfwTerminate();
}

void GlfwContextCreator::createContext()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// enable vertical sync
	glfwSwapInterval(1);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, mName.c_str(), NULL,
							   NULL);
	if (mWindow == NULL)
		throw std::runtime_error("Failed to create GLFW window");

	glfwMakeContextCurrent(mWindow);
}
void GlfwContextCreator::destroyContext()
{
	glfwDestroyWindow(mWindow);
}

} // namespace glcontext

} // namespace dream
