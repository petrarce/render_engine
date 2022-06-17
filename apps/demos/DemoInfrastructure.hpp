#include <src/components/Singleton.hpp>
#include <src/objects/camera/CameraController.hpp>
#include <GLGlfwContextCreator.hpp>
#include <map>
#include <Eigen/Dense>

static dream::geometry::GLCameraController controller;
static bool rotationEnabled	   = false;
static bool lastPositionCached = false;

class DemoInfrastructure
	: public dream::components::Singleton<DemoInfrastructure>
{
public:
	friend class dream::components::Singleton<DemoInfrastructure>;
	void run(const std::function<void(int, int)> &func)
	{

		while (!glfwContext.processWindow<int>(glfwWindowShouldClose))
		{
			int width, height;
			glfwContext.processWindow<void, int *, int *>(
				std::function<void(GLFWwindow *, int *, int *)>(
					glfwGetWindowSize),
				&width, &height);

			func(width, height);

			glfwContext.processWindow<void>(glfwSwapBuffers);
			glfwPollEvents();
		}
	}
	const dream::geometry::GLCameraController &getController() const
	{
		return controller;
	}

private:
	DemoInfrastructure()
		: glfwContext(800, 500)
		, mRenderFunction([]() { /*noop*/ })
		, mContextBinder(glfwContext)
	{
		controller.camera = dream::geometry::GLCamera(Eigen::Vector3f(0, 0, 1),
													  Eigen::Vector3f(1, 0, 0));
		controller.camera.pivot		= Eigen::Vector3f(0, 0, 0);
		controller.camera.distance	= 10;
		controller.camera.angleX	= -0.25;
		controller.camera.angleZ	= 0;
		controller.camera.upAxis	= Eigen::Vector3f(0, 0, 1);
		controller.camera.rightAxis = Eigen::Vector3f(1, 0, 0);

		//===================Set up callbacks======================

		glfwContext.processWindow<void, GLFWscrollfun>(
			std::function<void(GLFWwindow *, GLFWscrollfun)>(
				glfwSetScrollCallback),
			[](GLFWwindow *window, double dx, double dy)
			{ controller.camera.distance -= dy; });
		glfwContext.processWindow<void, GLFWmousebuttonfun>(
			std::function<void(GLFWwindow *, GLFWmousebuttonfun)>(
				glfwSetMouseButtonCallback),
			[](GLFWwindow *window, int button, int action, int mods)
			{
				std::map<int, std::function<void()>> pressActions = {
					{ GLFW_MOUSE_BUTTON_1,
					  []()
					  {
						  rotationEnabled	 = true;
						  lastPositionCached = false;
					  } }
				};
				std::map<int, std::function<void()>> releaseActins = {
					{ GLFW_MOUSE_BUTTON_1,
					  []()
					  {
						  rotationEnabled	 = false;
						  lastPositionCached = false;
					  } }
				};
				if (action == GLFW_PRESS)
				{
					if (pressActions.find(button) == pressActions.end())
						return;
					pressActions.at(button)();
				}
				if (action == GLFW_RELEASE)
				{
					if (releaseActins.find(button) == releaseActins.end())
						return;
					releaseActins.at(button)();
				}
			});
		glfwContext.processWindow<void, GLFWkeyfun>(
			std::function<void(GLFWwindow *, GLFWkeyfun)>(glfwSetKeyCallback),
			[](GLFWwindow *window, int key, int scancode, int action, int mods)
			{
				Eigen::Vector3f cameraViewDirection =
					-controller.camera.rightAxis.cross(
						controller.camera.upAxis);
				std::map<int, std::function<void()>> pressActions{
					{
						GLFW_KEY_ESCAPE,
						[&window]() { glfwSetWindowShouldClose(window, true); },
					},
					{
						GLFW_KEY_LEFT,
						[]() {
							controller.cameraMove(controller.camera.rightAxis *
												  -1);
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
						{ controller.cameraMove(-1.f * cameraViewDirection); },
					},
					{
						GLFW_MOUSE_BUTTON_1,
						[]() { rotationEnabled = true; },
					}
				};
				std::map<int, std::function<void()>> releaseActins{ {
					GLFW_MOUSE_BUTTON_1,
					[]() { rotationEnabled = false; },
				}

				};
				if (action == GLFW_PRESS)
				{
					if (pressActions.find(key) == pressActions.end())
						return;
					pressActions.at(key)();
				}
				if (action == GLFW_RELEASE)
				{
					if (releaseActins.find(key) == releaseActins.end())
						return;
					releaseActins.at(key)();
				}
			});
		glfwContext.processWindow<void, GLFWcursorposfun>(
			std::function<void(GLFWwindow *, GLFWcursorposfun)>(
				glfwSetCursorPosCallback),
			[](GLFWwindow *window, double posX, double posY)
			{
				static double lastPosX;
				static double lastPosY;
				if (!rotationEnabled)
					return;

				if (!lastPositionCached)
				{
					lastPosX		   = posX;
					lastPosY		   = posY;
					lastPositionCached = true;
				}

				double dx = posX - lastPosX;
				double dy = posY - lastPosY;

				controller.cameraRotate(0.01 * -dy, 0.01 * -dx);
				lastPosX = posX;
				lastPosY = posY;
			});

		//===================end callbacks======================
	}

	dream::glcontext::GLGlfwContextCreator glfwContext;
	std::function<void()> mRenderFunction;
	dream::glcontext::GLContextBinder mContextBinder;
	bool mRotateEnabled = false;
};
