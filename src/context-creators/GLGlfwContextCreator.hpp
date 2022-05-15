#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "GLContextCreator.hpp"
#include <functional>
namespace dream
{

namespace glcontext
{
class GLGlfwContextCreator : public GLContextCreator
{
public:
	GLGlfwContextCreator(int width = 1, int height = 1,
						 const std::string &name = "GLGlfwContextCreator");
	virtual ~GLGlfwContextCreator();

	template <class RetType, class... Args>
	RetType
	processWindow(std::function<RetType(GLFWwindow *, Args...)> processor,
				  Args... args)
	{
		if (!processor || !mWindow)
			throw std::runtime_error("Cannot process current window");
		return processor(mWindow, args...);
	}

	template <class RetType>
	RetType processWindow(std::function<RetType(GLFWwindow *)> processor)
	{
		if (!processor || !mWindow)
			throw std::runtime_error("Cannot process current window");
		return processor(mWindow);
	}

	void setHint(size_t hint, size_t value)
	{
		glfwWindowHint(hint, value);
	}

	int mWindowWidth{ 0 };
	int mWindowHeight{ 0 };

private:
	void createContext() final;
	void destroyContext() final;

	GLFWwindow *mWindow;
};

} // namespace glcontext

} // namespace dream
