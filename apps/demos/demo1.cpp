#include <GLWrapperCore>
#include <GLGeometryObjects>
#include <GLGlfwContextCreator.hpp>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

std::string vertexShader = "\
	#version 330 core\n \
	layout (location = 0) in vec3 aPos;\n \
	layout (location = 1) in vec3 aColor;\n \
	 \
	uniform mat4 projection;\n									\
	uniform mat4 view;\n \
	uniform mat4 model;\n \
	out vec3 ourColor;\n \
	void main()\n \
	{\n \
		gl_Position = projection * view * model * vec4(aPos, 1.0);\
		ourColor = aColor;\n\
	}\n\
	";
std::string fragmentShader = "\
		#version 330 core							   \n\
		out vec4 FragColor;							   \n\
													   \n\
		in vec3 ourColor;							   \n\
													   \n\
		void main()									   \n\
		{											   \n\
			FragColor = vec4(ourColor, 1.0f);		   \n\
		}											   \n\
		";
std::vector<float> vertices = {
		-0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f, -0.5f,  0.34, 0.96, 0.24,
		-0.5f,  0.5f, -0.5f,  0.34, 0.96, 0.24,
		-0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,

		-0.5f, -0.5f,  0.5f,  0.34, 0.96, 0.24,
		 0.5f, -0.5f,  0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,
		-0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,
		-0.5f, -0.5f,  0.5f,  0.34, 0.96, 0.24,

		-0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,
		-0.5f,  0.5f, -0.5f,  0.34, 0.96, 0.24,
		-0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,
		-0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,
		-0.5f, -0.5f,  0.5f,  0.34, 0.96, 0.24,
		-0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,

		 0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f, -0.5f,  0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,

		-0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f, -0.5f,  0.5f,  0.34, 0.96, 0.24,
		 0.5f, -0.5f,  0.5f,  0.34, 0.96, 0.24,
		-0.5f, -0.5f,  0.5f,  0.34, 0.96, 0.24,
		-0.5f, -0.5f, -0.5f,  0.34, 0.96, 0.24,

		-0.5f,  0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f, -0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,
		 0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,
		-0.5f,  0.5f,  0.5f,  0.34, 0.96, 0.24,
		-0.5f,  0.5f, -0.5f,  0.34, 0.96, 0.24,
	};

object::camera::GLCameraController controller;

int main()
{
	using namespace glcontext::creators;
	using namespace glwrapper::core;
	using namespace object::camera;
	GLGlfwContextCreator glfwContext(800, 500);
	GLContextBinder bind(glfwContext);
	controller.camera.pivot = Eigen::Vector3f(0, 0, 0);
	controller.camera.distance = 10;
	controller.camera.angleX = -0.25;
	controller.camera.angleZ = 0;

	//===================Set up callbacks======================

	glfwContext.processWindow<void, GLFWscrollfun>(
		std::function<void(GLFWwindow*, GLFWscrollfun)>(glfwSetScrollCallback),
		[](GLFWwindow* window, double dx, double dy)
		{
			controller.camera.distance -= dy;
		});

	glfwContext.processWindow<void, GLFWkeyfun>(
		std::function<void(GLFWwindow*, GLFWkeyfun)>(glfwSetKeyCallback),
		[/*&controller*/](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			Eigen::Vector3f cameraViewDirection = -controller.camera.rightAxis.cross(controller.camera.upAxis);
			std::map<int, std::function<void()>> pressActions {
				{GLFW_KEY_ESCAPE, [&window]() { glfwSetWindowShouldClose(window, true); }},
				{GLFW_KEY_LEFT, [](){ controller.cameraMove(controller.camera.rightAxis  * -1);}},
				{GLFW_KEY_RIGHT, [](){ controller.cameraMove(controller.camera.rightAxis);}},
				{GLFW_KEY_UP, [&cameraViewDirection](){ controller.cameraMove(cameraViewDirection);}},
				{GLFW_KEY_DOWN, [&cameraViewDirection](){ controller.cameraMove(-cameraViewDirection);}},
			};
			if(action == GLFW_PRESS)
			{
				if(pressActions.find(key) == pressActions.end())
					return;
				pressActions.at(key)();
			}
		});
	glfwContext.processWindow<void, GLFWcursorposfun>(
		std::function<void(GLFWwindow*, GLFWcursorposfun)>(glfwSetCursorPosCallback),
		[/*&controller*/](GLFWwindow* window, double posX, double posY)
		{
			static double lastPosX;
			static double lastPosY;
			static bool initialized = false;

			if(!initialized)
			{
				lastPosX = posX;
				lastPosY = posY;
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
	GLShaderProgram prog;
	prog.prepare(vertexShader, fragmentShader);
	GLArrayBuffer buf;
	buf.create(vertices, GL_STATIC_DRAW);
	GLVertexArray va;
	GLVertexArray::AttributeSpecification vertSpec = {
		.components = 3,
		.location = 0,
		.normalize = false,
		.offset = 0,
		.stride = 6 * sizeof(float),
		.type = GL_FLOAT
	};
	auto colorSpec = vertSpec;
	colorSpec.location = 1; colorSpec.offset = 3 * sizeof(float);
	va.createAttribute(vertSpec, buf);
	va.createAttribute(colorSpec, buf);

	prog.use();
	prog.setMatrix("model", Eigen::Matrix4f(Eigen::Matrix4f::Identity()));
	//==================end opengl state initialization===================

	// glfw: initialize and configure
	// ------------------------------
	while (!glfwContext.processWindow<int>(glfwWindowShouldClose))
	{
		glClearColor(0.2345f, 0.492f, 0.717f, 1.f);
//		glClearDepth(1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		int width, height;
		glfwContext.processWindow<void, int*, int*>(std::function<void(GLFWwindow*, int*, int*)>(glfwGetWindowSize), &width, &height);
		glm::mat4 p = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
		Eigen::Matrix4f projectionE(&p[0][0]);
		prog.setMatrix("projection", projectionE);
		prog.setMatrix("view", controller.camera.toViewTransform());

		// input
		// -----
		GlObjectBinder bindBuf(va);
		glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse
		// moved etc.)
		// -------------------------------------------------------------------------------
		glfwContext.processWindow<void>(glfwSwapBuffers);
		glfwPollEvents();
	}
	return 0;
}
