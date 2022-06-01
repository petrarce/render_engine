#include <GLWrapperCore>
#include <GLGeometryObjects>
#include <GLComponents>
#include <GLGlfwContextCreator.hpp>
#include "DemoInfrastructure.hpp"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class DrawScreenRectangle : dream::components::GLTriangleDrawer
{
public:
	DrawScreenRectangle()
	{
		mVAB.create(
			std::vector<float>{
				-1,
				-1,
				0,
				1,
				-1,
				0,
				-1,
				1,
				0,
				-1,
				1,
				0,
				1,
				-1,
				0,
				1,
				1,
				0,
			},
			GL_STATIC_DRAW);
	}
	void draw(const dream::components::Scope &scp)
	{
		using namespace molecular::util;

		dream::components::Scope myScope(scp);
		myScope.Set("aVerPos"_H,
					dream::components::Attribute<Eigen::Vector3f>());

		mProgram.generate(myScope);
		mProgram.prepare(myScope);

		dream::glwrapper::GLObjectBinder bindProg(mProgram);
		dream::glwrapper::GLObjectBinder binVAO(mVAO);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
};

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

	DrawScreenRectangle screenRectangleDrawer;
	dream::glwrapper::GLFrameBufferObject fbo;
	dream::glwrapper::GLTexture2D screenTexture;
	dream::glwrapper::GLRenderBuffer depthRenderBuffer;

	screenTexture.create<2>(0,
							dream::glwrapper::GLTexture2D::InternalFormat::Rgba,
							std::array<std::size_t, 2>({ 2000, 2000 }));
	depthRenderBuffer.create(2000, 2000, GL_DEPTH);
	fbo.attach(dream::glwrapper::GLFrameBufferObject::AttachmentPoint::Color0,
			   screenTexture);
	fbo.attach(dream::glwrapper::GLFrameBufferObject::Depth, depthRenderBuffer);

	//==================end opengl state initialization===================

	// glfw: initialize and configure
	// ------------------------------
	auto renderRoutine = [&](int width, int height)
	{
		using namespace dream::components;
		using namespace molecular::util;
		auto &controller = infra.getController();
		{
			dream::glwrapper::GLDrawFramebufferBinder bindFBO(fbo);
			glClearColor(0.2345f, 0.492f, 0.717f, 1.f);
			//		glClearDepth(1.f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClear(GL_DEPTH_BUFFER_BIT);
			//===============Rendering loop=====================

			glm::mat4 p =
				glm::perspective(glm::radians(45.0f),
								 (float)width / (float)height, 0.1f, 1000.0f);
			Eigen::Matrix4f projectionE(&p[0][0]);
			rootScope.Set<Uniform<Eigen::Matrix4f>>(
				"uProjection"_H, Uniform<Eigen::Matrix4f>(projectionE));
			rootScope.Set<Uniform<Eigen::Matrix4f>>(
				"uView"_H,
				Uniform<Eigen::Matrix4f>(controller.camera.toViewTransform()));
			std::cerr << controller.camera.toViewTransform() << std::endl;
			multipleDraws.draw(rootScope);
		}

		dream::components::Scope screenScope;
		screenScope.Set("uScreenTexture"_H,
						dream::components::Uniform<int>(screenTexture.getId()));
		screenRectangleDrawer.draw(screenScope);
	};
	infra.run(std::ref(renderRoutine));

	return 0;
}
