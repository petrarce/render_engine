#include "GLGlfwContextCreator.hpp"
#include <string>
#include <iostream>

namespace dream
{

namespace glcontext
{
GLGlfwContextCreator::GLGlfwContextCreator(int width, int height,
										   const std::string &name)
	: GLContextCreator(name)
	, mWindowWidth(width)
	, mWindowHeight(height)
{
	glfwInit();
}
GLGlfwContextCreator::~GLGlfwContextCreator()
{
	glfwTerminate();
}

void GLGlfwContextCreator::createContext()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	glfwSetFramebufferSizeCallback(
		mWindow, [](GLFWwindow * /*window*/, int width, int height)
		{ glViewport(0, 0, width, height); });

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize GLAD");
}
void GLGlfwContextCreator::destroyContext()
{
	glfwDestroyWindow(mWindow);
}

} // namespace glcontext

} // namespace dream
