#include "GLArrayBufferDrawer.hpp"
#include <array>
namespace dream
{
namespace components
{

GLTriangleDrawer::GLTriangleDrawer()
	: GLRenderFunction()
{
	std::vector<float> vertices = {
		-1, 0, 0, 0, 1, 0, 1, 0, 0,
	};
	mVAB.create(vertices,
				glwrapper::GLArrayBuffer::MutableStorageType::StaticDraw);
	glwrapper::GLVertexArray::AttributeSpecification spec = {
		.components	   = 3,
		.instanceLevel = 0,
		.location	   = 0,
		.normalize	   = false,
		.offset		   = 0,
		.stride		   = sizeof(float) * 3,
		.type		   = GL_FLOAT,
	};
	mVAO.createAttribute(spec, mVAB);
}

void GLTriangleDrawer::draw(const Scope &parentScope)
{
	using namespace molecular::util;
	Scope currentScope(parentScope);
	currentScope.Set("aVerPos"_H, Attribute<float>());
	currentScope.Set("uModel"_H,
					 Uniform<Eigen::Matrix4f>(Eigen::Matrix4f::Identity()));
	currentScope.Set("uColor"_H,
					 Uniform<std::array<float, 4>>({ 0.8f, 0.5f, 0.f, 1.f }));
	mProgram.generateProgram(currentScope);
	mProgram.prepareProgram(currentScope);

	mProgram.use();
	//	glwrapper::GlObjectBinder bindProgram(mProgram);
	glwrapper::GlObjectBinder bindVAO(mVAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

} // namespace components
} // namespace dream
