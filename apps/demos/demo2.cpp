#include <GLWrapperCore>
#include <GLGeometryObjects>
#include <GLComponents>
#include <GLRenderer>
#include <GLGlfwContextCreator.hpp>
#include <string>
#include <molecular/util/Hash.h>
#include "DemoInfrastructure.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace dream::renderer;
using namespace dream::components;
using namespace dream::glwrapper;
using namespace molecular::util;

int main(int argc, char **argv)
{
	DemoInfrastructure &infra = DemoInfrastructure::instance();

	//===================setup rendergraph =========
	auto view = std::make_shared<GLViewSetupRenderableObject>();
	auto meshScene =
		std::make_shared<GLRenderableObject>("MeshSceneRenderObject");
	auto cube		  = std::make_shared<GLCubeObject>();
	auto rootRenderer = std::make_shared<GLScreenTextureRenderObject>();
	meshScene->addChild(cube);
	auto rectangleMesh = std::make_shared<GLMeshWithMaterialObject::Mesh>();
	rectangleMesh->vertices = std::vector<float>(
		{ -1, -1, 0, -1, 1, 0, 1, -1, 0, 1, -1, 0, -1, 1, 0, 1, 1, 0 });
	rectangleMesh->textures = { 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1 };
	const int rows			= argc == 2 ? std::stoi(argv[1]) : 5;
	for (int i = 0; i < rows * rows; i++)
	{
		Eigen::Matrix4f transform = Eigen::Matrix4f::Identity();

		transform.block<3, 1>(0, 3) << (i % rows) * 5, (i / rows) * 5, 0;
		auto meshWithMaterial = std::make_shared<GLMeshWithMaterialObject>(
			"Mesh" + std::to_string(i));
		meshWithMaterial->setTransform(transform);
		meshWithMaterial->setAmbient(GLMeshWithMaterialRenderFunction::Texture{
			"../assets/Grass.png", GLTexture2D::InternalFormat::Rgba });
		meshWithMaterial->setMesh(rectangleMesh);
		meshScene->addChild(meshWithMaterial);
	}
	view->addChild(meshScene);
	rootRenderer->addChild(view);

	//===================setup root scope =========
	dream::components::Scope rootScope;
	{
		using namespace dream::components;
		using namespace molecular::util;
		rootScope.Set("fragColor"_H, dream::components::Output());
		rootScope.Set("gl_Position"_H, dream::components::Output());
	}

	view->setFarPlane(1000.f);
	view->setNearPlane(0.1f);
	view->setFov(45.f);

	auto renderRoutine = [&](int width, int height)
	{
		using namespace dream::components;
		using namespace molecular::util;

		std::array<int, 4> m_viewport;
		glGetIntegerv(GL_VIEWPORT, m_viewport.data());

		static int frame = 0;
		frame++;
		Eigen::Matrix4f cubeTransform = Eigen::Matrix4f::Identity();
		cubeTransform.block<3, 3>(0, 0) =
			Eigen::AngleAxisf(frame / 180.f * M_PI / 5.f,
							  Eigen::Vector3f(0, 0, 1))
				.matrix();
		cube->setTransform(cubeTransform);
		view->setCamera(infra.getController().camera);
		view->setAspectRatio(static_cast<float>(m_viewport[2]) / m_viewport[3]);
		rootRenderer->setViewport(m_viewport);
		rootRenderer->resetSync();
		rootRenderer->sync();
		rootRenderer->renderFunction()->draw(rootScope);
	};

	infra.run(std::ref(renderRoutine));

	return 0;
}
