#include <GLWrapperCore>
#include <GLGeometryObjects>
#include <GLComponents>
#include <GLGlfwContextCreator.hpp>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

dream::geometry::GLCameraController controller;

int main()
{
	using namespace dream::glcontext;
	using namespace dream::glwrapper;
	using namespace dream::geometry;
	using namespace dream::components;
	GLGlfwContextCreator glfwContext(800, 500);
	GLContextBinder bind(glfwContext);
	controller.camera.pivot	   = Eigen::Vector3f(0, 0, 0);
	controller.camera.distance = 10;
	controller.camera.angleX   = -0.25;
	controller.camera.angleZ   = 0;

	//===================Set up callbacks======================

	glfwContext.processWindow<void, GLFWscrollfun>(
		std::function<void(GLFWwindow *, GLFWscrollfun)>(glfwSetScrollCallback),
		[](GLFWwindow *window, double dx, double dy)
		{ controller.camera.distance -= dy; });

	glfwContext.processWindow<void, GLFWkeyfun>(
		std::function<void(GLFWwindow *, GLFWkeyfun)>(glfwSetKeyCallback),
		[/*&controller*/](GLFWwindow *window, int key, int scancode, int action,
						  int mods)
		{
			Eigen::Vector3f cameraViewDirection =
				-controller.camera.rightAxis.cross(controller.camera.upAxis);
			std::map<int, std::function<void()>> pressActions{
				{
					GLFW_KEY_ESCAPE,
					[&window]() { glfwSetWindowShouldClose(window, true); },
				},
				{
					GLFW_KEY_LEFT,
					[]() {
						controller.cameraMove(controller.camera.rightAxis * -1);
					},
				},
				{
					GLFW_KEY_RIGHT,
					[]()
					{ controller.cameraMove(controller.camera.rightAxis); },
				},
				{
					GLFW_KEY_UP,
					[&cameraViewDirection]()
					{ controller.cameraMove(cameraViewDirection); },
				},
				{
					GLFW_KEY_DOWN,
					[&cameraViewDirection]()
					{ controller.cameraMove(-cameraViewDirection); },
				},
			};
			if (action == GLFW_PRESS)
			{
				if (pressActions.find(key) == pressActions.end())
					return;
				pressActions.at(key)();
			}
		});
	glfwContext.processWindow<void, GLFWcursorposfun>(
		std::function<void(GLFWwindow *, GLFWcursorposfun)>(
			glfwSetCursorPosCallback),
		[/*&controller*/](GLFWwindow *window, double posX, double posY)
		{
			static double lastPosX;
			static double lastPosY;
			static bool initialized = false;

			if (!initialized)
			{
				lastPosX	= posX;
				lastPosY	= posY;
				initialized = true;
			}

			double dx = posX - lastPosX;
			double dy = posY - lastPosY;

			controller.cameraRotate(0.01 * -dy, 0.01 * -dx);
			lastPosX = posX;
			lastPosY = posY;
		});

	//===================end callbacks======================

	//===================initialize opengl objects =========
	GLTriangleDrawer triangle;
	Scope rootScope;
	{
		using namespace molecular::util;
		rootScope.Set("fragColor"_H, Output());
		rootScope.Set("gl_Position"_H, Output());
	}
	//==================end opengl state initialization===================

	// glfw: initialize and configure
	// ------------------------------
	while (!glfwContext.processWindow<int>(glfwWindowShouldClose))
	{
		using namespace molecular::util;
		glClearColor(0.2345f, 0.492f, 0.717f, 1.f);
		//		glClearDepth(1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		//===============Rendering loop=====================
		int width, height;
		glfwContext.processWindow<void, int *, int *>(
			std::function<void(GLFWwindow *, int *, int *)>(glfwGetWindowSize),
			&width, &height);

		glm::mat4 p = glm::perspective(
			glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
		Eigen::Matrix4f projectionE(&p[0][0]);
		rootScope.Set<Uniform<Eigen::Matrix4f>>(
			"uProjection"_H, Uniform<Eigen::Matrix4f>(projectionE));
		rootScope.Set<Uniform<Eigen::Matrix4f>>(
			"uView"_H,
			Uniform<Eigen::Matrix4f>(controller.camera.toViewTransform()));
		std::cerr << controller.camera.toViewTransform() << std::endl;
		triangle.draw(rootScope);
		//=============== End Rendering loop=====================
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse
		// moved etc.)
		// -------------------------------------------------------------------------------
		glfwContext.processWindow<void>(glfwSwapBuffers);
		glfwPollEvents();
	}
	return 0;
}
