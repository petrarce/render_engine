#include "OpenGLGlfwContextCreator.h"
namespace dream
{
namespace glcontext
{

OpenGLGlfwContextCreator::OpenGLGlfwContextCreator(int width, int height,
												   const std::string &name)
	: GlfwContextCreator(width, height, name)
{
}

void OpenGLGlfwContextCreator::createContext()
{
	GlfwContextCreator::createContext();
	glfwSetFramebufferSizeCallback(
		mWindow, [](GLFWwindow * /*window*/, int width, int height)
		{ glViewport(0, 0, width, height); });

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Failed to initialize GLAD");

	const char *version =
		reinterpret_cast<const char *>(glGetString(GL_VERSION));
	std::cerr << "Using next OpenGL version: " << version << std::endl;
}

} // namespace glcontext
} // namespace dream
