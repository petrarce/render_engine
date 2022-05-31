#include "GLCubeStripRenderFunction.hpp"
#include <molecular/util/Hash.h>
#include <Eigen/Dense>
#include <array>

namespace dream
{
namespace components
{

GLCubeStripRenderFunction::GLCubeStripRenderFunction()
	: GLRenderFunction()
{
	std::vector<float> cubeStrip = {
		-1.f, 1.f,	1.f,  // Front-top-left
		1.f,  1.f,	1.f,  // Front-top-right
		-1.f, -1.f, 1.f,  // Front-bottom-left
		1.f,  -1.f, 1.f,  // Front-bottom-right
		1.f,  -1.f, -1.f, // Back-bottom-right
		1.f,  1.f,	1.f,  // Front-top-right
		1.f,  1.f,	-1.f, // Back-top-right
		-1.f, 1.f,	1.f,  // Front-top-left
		-1.f, 1.f,	-1.f, // Back-top-left
		-1.f, -1.f, 1.f,  // Front-bottom-left
		-1.f, -1.f, -1.f, // Back-bottom-left
		1.f,  -1.f, -1.f, // Back-bottom-right
		-1.f, 1.f,	-1.f, // Back-top-left
		1.f,  1.f,	-1.f  // Back-top-right
	};
	mVBO.createImmutable(cubeStrip,
						 glwrapper::GLArrayBuffer::ImmutableStorageType::Read);
	mVAO.createAttribute(
		glwrapper::GLVertexArray::AttributeSpecification{
			.components	   = 3,
			.instanceLevel = 0,
			.location	   = 0,
			.normalize	   = false,
			.offset		   = 0,
			.stride		   = sizeof(float) * 3,
			.type		   = GL_FLOAT,
		},
		mVBO);
}

void GLCubeStripRenderFunction::draw(const dream::components::Scope &scp)
{
	using namespace ::molecular::util;
	dream::components::Scope currentScope(scp);

	Eigen::Matrix4f translation = Eigen::Matrix4f::Identity();
	translation.block<3, 1>(0, 3) << 5, 6, 7;
	currentScope.Set("aVerPos"_H, Attribute<float>());
	currentScope.Set("uModel"_H, Uniform<Eigen::Matrix4f>(translation));
	currentScope.Set("uColor"_H,
					 Uniform<std::array<float, 4>>({ 0.f, 0.5f, 0.2f, 1.f }));

	mProg.generateProgram(currentScope);
	mProg.prepareProgram(currentScope);

	dream::glwrapper::GLObjectBinder bindVAO(mVAO);
	dream::glwrapper::GLObjectBinder bindProg(mProg);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 42);
}

} // namespace components
} // namespace dream
