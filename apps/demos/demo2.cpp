#include <GLWrapperCore>
#include <GLGeometryObjects>
#include <GLComponents>
#include <GLRenderer>
#include <GLGlfwContextCreator.hpp>
#include "DemoInfrastructure.hpp"
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace dream::renderer;
using namespace dream::components;
using namespace dream::glwrapper;
class DrawScreenRectangle
{
public:
	DrawScreenRectangle()
	{
		mVAB.create(
			std::vector<float>{
				-1, -1, 0,	0, 0,
				1, -1, 0,	1, 0,
				-1, 1, 0,	0, 1,
				1, 1, 0,	1, 1,
			},
			GL_STATIC_DRAW);
		mEAB.create(std::vector<unsigned int>{ 0, 1, 2, 2, 1, 3 },
					GL_STATIC_DRAW);
		// vertex position attribute
		mVAO.createAttribute(
			dream::glwrapper::GLVertexArray::AttributeSpecification{
				.components	   = 3,
				.instanceLevel = 0,
				.location	   = 0,
				.normalize	   = false,
				.offset		   = 0,
				.stride		   = sizeof(float) * 5,
				.type		   = GL_FLOAT,
			},
			mVAB, mEAB);
		// texture coordinates attribute
		mVAO.createAttribute(
			dream::glwrapper::GLVertexArray::AttributeSpecification{
				.components	   = 2,
				.instanceLevel = 0,
				.location	   = 1,
				.normalize	   = false,
				.offset		   = sizeof(float) * 3,
				.stride		   = sizeof(float) * 5,
				.type		   = GL_FLOAT,
			},
			mVAB, mEAB);
	}
	void draw(const dream::components::Scope &scp)
	{
		using namespace molecular::util;

		dream::components::Scope myScope(scp);
		myScope.Set("aVerPos"_H,
					dream::components::Attribute<Eigen::Vector3f>());
		myScope.Set("aTexCoord"_H,
					dream::components::Attribute<Eigen::Vector2f>());

		mProgram.generate(myScope);
		mProgram.prepare(myScope);

		dream::glwrapper::GLObjectBinder bindProg(mProgram);
		dream::glwrapper::GLObjectBinder binVAO(mVAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
	dream::glwrapper::GLElementArrayBuffer mEAB;
	dream::glwrapper::GLArrayBuffer mVAB;
	dream::glwrapper::GLVertexArray mVAO;
	dream::components::GLMolecularProgram mProgram;
};

int main()
{
	DemoInfrastructure &infra = DemoInfrastructure::instance();

	//===================initialize opengl objects =========
	//	dream::components::GLMultipleCaleeRenderFunction multipleDraws({
	//		std::make_shared<dream::components::GLTriangleDrawer>(),
	//		std::make_shared<dream::components::GLCubeStripRenderFunction>(),
	//	});
	std::shared_ptr<GLViewSetupRenderableObject> view(
		new GLViewSetupRenderableObject());
	std::shared_ptr<GLRenderableObject> meshScene(
		new GLRenderableObject("MeshSceneRenderObject"));
	std::shared_ptr<GLCubeObject> cube(new GLCubeObject("CubeRenderObject"));
	meshScene->addChild(std::static_pointer_cast<GLRenderableObject>(cube));
	view->addChild(meshScene);
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
	screenTexture.setParameter(
		dream::glwrapper::GLTexture2D::ValueMinFilter::MinLinear);
	screenTexture.setParameter(
		dream::glwrapper::GLTexture2D::ValueMagFilter::MagLinear);
	depthRenderBuffer.create(2000, 2000, GL_DEPTH24_STENCIL8);
	{
		dream::glwrapper::GLDrawFramebufferBinder bindFBO(fbo);
		fbo.attach(
			dream::glwrapper::GLFrameBufferObject::AttachmentPoint::Color0,
			screenTexture);
		fbo.attach(dream::glwrapper::GLFrameBufferObject::DepthStencil,
				   depthRenderBuffer);
		if (bindFBO.state() !=
			dream::glwrapper::GLDrawFramebufferBinder::Complete)
			throw std::runtime_error(
				"Failed to initialize screen renderbuffer error: " +
				std::to_string(bindFBO.state()));
	}

	view->setFarPlane(1000.f);
	view->setNearPlane(0.1f);
	view->setFov(45.f);

	//=================20=end opengl state initialization===================

	// glfw: initialize and configure
	// ------------------------------
	auto renderRoutine = [&](int width, int height)
	{
		using namespace dream::components;
		using namespace molecular::util;
		GLint m_viewport[4];
		glGetIntegerv(GL_VIEWPORT, m_viewport);
		auto &controller = infra.getController();
		{
			dream::glwrapper::GLDrawFramebufferBinder bindFBO(fbo);

			glEnable(GL_DEPTH_TEST);
			glViewport(0, 0, 2000, 2000);
			view->setAspectRatio(2000 / 2000);
			glClearColor(0.2345f, 0.492f, 0.717f, 1.f);
			//		glClearDepth(1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//===============Rendering loop=====================
			static int frame = 0;
			frame++;
			Eigen::Matrix4f cubeTransform = Eigen::Matrix4f::Identity();
			cubeTransform.block<3, 3>(0, 0) =
				Eigen::AngleAxisf(frame / 180.f * M_PI / 5.f,
								  Eigen::Vector3f(0, 0, 1))
					.matrix();
			cube->setTransform(cubeTransform);
			view->setCamera(controller.camera);
			view->resetSync();
			view->sync();
			view->renderFunction()->draw(rootScope);
		}
		glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
		glDisable(GL_DEPTH_TEST);
		glClearColor(
			1.0f, 1.0f, 1.0f,
			1.0f); // set clear color to white (not really necessary actually,
				   // since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);
		dream::glwrapper::GLTextureUnit textUnit(GL_TEXTURE0);
		dream::glwrapper::GLObjectBinder textureBinder(screenTexture);
		dream::components::Scope screenScope(rootScope);
		screenTexture.attach(dream::glwrapper::GLTextureUnit::Texture0 + 1);
		screenScope.Set("uScreenTexture"_H, dream::components::Uniform<int>(1));
		screenRectangleDrawer.draw(screenScope);
	};

	infra.run(std::ref(renderRoutine));

	return 0;
}
