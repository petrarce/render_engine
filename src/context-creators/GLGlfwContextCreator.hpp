#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include "GLContextCreator.hpp"
namespace dream
{

namespace glcontext
{
class GlfwContextCreator : public ContextCreator
{
public:
	GlfwContextCreator(int width = 1, int height = 1,
					   const std::string &name = "GLGlfwContextCreator");
	virtual ~GlfwContextCreator();

	template <class RetType, class... Args>
	RetType processWindow(
		const std::function<RetType(GLFWwindow *, Args...)> &processor,
		Args... args)
	{
		if (!processor || !mWindow)
			throw std::runtime_error("Cannot process current window");
		return processor(mWindow, args...);
	}

	template <class RetType>
	RetType processWindow(const std::function<RetType(GLFWwindow *)> &processor)
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

protected:
	void createContext() override;
	void destroyContext() override;

	GLFWwindow *mWindow;
};

} // namespace glcontext

} // namespace dream
