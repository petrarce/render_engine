#pragma once
#include <GLWrapperCore>
#include <GLComponents>
#include <GLRenderableObject.hpp>
namespace dream
{
namespace components
{
class GLCubeRenderFunction : public GLMultipleCaleeRenderFunction
{
	GLCubeRenderFunction()
		: GLMultipleCaleeRenderFunction({})
	{
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
	~GLCubeRenderFunction();
	void draw(const Scope &parentScope)
	{
		Scope currentScope(parentScope);
		currentScope.Set(molecular::util::HashUtils::MakeHash("aVerPos"),
						 Attribute<Eigen::Vector3f>());
		currentScope.Set(molecular::util::HashUtils::MakeHash("uColor"),
						 Uniform<Eigen::Vector4f>(mColor));
		mProgram.generate(currentScope);
		mProgram.prepare(currentScope);

		glwrapper::GLObjectBinder bindVAO(mVAO);
		glwrapper::GLObjectBinder bindProgram(mProgram);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 42);
	}
	void updateCube(const Eigen::Vector3f &size)
	{
		std::vector<float> cubeStrip = {
			-size(0)/2.f,	 size(1)/2.f,	 size(2)/2.f,  // Front-top-left
			 size(0)/2.f,	 size(1)/2.f,	 size(2)/2.f,  // Front-top-right
			-size(0)/2.f,	 size(1)/2.f,	 size(2)/2.f,  // Front-bottom-left
			 size(0)/2.f,	 size(1)/2.f,	 size(2)/2.f,  // Front-bottom-right
			 size(0)/2.f,	 size(1)/2.f,	-size(2)/2.f, // Back-bottom-right
			 size(0)/2.f,	 size(1)/2.f,	 size(2)/2.f,  // Front-top-right
			 size(0)/2.f,	 size(1)/2.f,	-size(2)/2.f, // Back-top-right
			-size(0)/2.f,	 size(1)/2.f,	 size(2)/2.f,  // Front-top-left
			-size(0)/2.f,	 size(1)/2.f,	-size(2)/2.f, // Back-top-left
			-size(0)/2.f,	-size(1)/2.f,	 size(2)/2.f,  // Front-bottom-left
			-size(0)/2.f,	-size(1)/2.f,	-size(2)/2.f, // Back-bottom-left
			 size(0)/2.f,	-size(1)/2.f,	-size(2)/2.f, // Back-bottom-right
			-size(0)/2.f,	 size(1)/2.f,	-size(2)/2.f, // Back-top-left
			 size(0)/2.f,	 size(1)/2.f,	-size(2)/2.f  // Back-top-right
		};
		mVBO.create(cubeStrip, GL_STATIC_DRAW);
	}

private:
	glwrapper::GLVertexArray mVAO;
	glwrapper::GLArrayBuffer mVBO;
	components::GLMolecularProgram mProgram;
	Eigen::Vector3f mColor;
};
} // namespace components
} // namespace dream

namespace dream
{
namespace renderer
{
class GLCubeObject : protected GLRenderableObject
{
	GLCubeObject(const std::string &name = "GLCubeObject")
		: GLRenderableObject(name)
	{
	}

private:
	Eigen::Vector3f mSize;
};
} // namespace renderer
} // namespace dream