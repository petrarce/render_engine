#include <GLWrapperCore>
#include <GLGeometryObjects>
#include <GLComponents>
#include <GLGlfwContextCreator.hpp>
#include "DemoInfrastructure.hpp"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

int main()
{
	DemoInfrastructure &infra = DemoInfrastructure::instance();

	//===================initialize opengl objects =========
	dream::components::GLMultipleCaleeRenderFunction multipleDraws({
		std::make_shared<dream::components::GLTriangleDrawer>(),
		std::make_shared<dream::components::GLCubeStripRenderFunction>(),
	});
	dream::components::Scope rootScope;
	{
		using namespace dream::components;
		using namespace molecular::util;
		rootScope.Set("fragColor"_H, dream::components::Output());
		rootScope.Set("gl_Position"_H, dream::components::Output());
	}
	//==================end opengl state initialization===================

	// glfw: initialize and configure
	// ------------------------------
	auto renderRoutine = [&](int width, int height)
	{
		using namespace dream::components;
		using namespace molecular::util;
		auto &controller = infra.getController();
		glClearColor(0.2345f, 0.492f, 0.717f, 1.f);
		//		glClearDepth(1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		//===============Rendering loop=====================

		glm::mat4 p = glm::perspective(
			glm::radians(45.0f), (float)width / (float)height, 0.1f, 1000.0f);
		Eigen::Matrix4f projectionE(&p[0][0]);
		rootScope.Set<Uniform<Eigen::Matrix4f>>(
			"uProjection"_H, Uniform<Eigen::Matrix4f>(projectionE));
		rootScope.Set<Uniform<Eigen::Matrix4f>>(
			"uView"_H,
			Uniform<Eigen::Matrix4f>(controller.camera.toViewTransform()));
		std::cerr << controller.camera.toViewTransform() << std::endl;
		multipleDraws.draw(rootScope);
	};
	infra.run(std::ref(renderRoutine));

	return 0;
}
