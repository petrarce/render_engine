#pragma once
#include <GLWrapperCore>
#include <GLComponents>
#include <GLRenderableObject.hpp>
#include <GLTransformedObject.hpp>
namespace dream
{
namespace components
{
class GLCubeRenderFunction : public GLTransformedRenderFunction
{
public:
	GLCubeRenderFunction()
		: GLTransformedRenderFunction()
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
	virtual ~GLCubeRenderFunction()
	{
	}
	void draw(const Scope &parentScope) override
	{
		Scope scope(parentScope);
		prepareScope(scope);

		mProgram.generate(scope);
		mProgram.prepare(scope);

		glwrapper::GLObjectBinder bindVAO(mVAO);
		glwrapper::GLObjectBinder bindProgram(mProgram);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 42);
	}
	void updateCube(const Eigen::Vector3f &size)
	{
		std::vector<float> cubeStrip = {
			-size(0)/2,	 size(1)/2,	 size(2)/2,     // Front-top-left
			 size(0)/2,	 size(1)/2,	 size(2)/2,      // Front-top-right
			-size(0)/2,	-size(1)/2,	 size(2)/2,    // Front-bottom-left
			 size(0)/2,	-size(1)/2,	 size(2)/2,     // Front-bottom-right
			 size(0)/2,	-size(1)/2,	-size(2)/2,    // Back-bottom-right
			 size(0)/2,	 size(1)/2,	 size(2)/2,      // Front-top-right
			 size(0)/2,	 size(1)/2,	-size(2)/2,     // Back-top-right
			-size(0)/2,	 size(1)/2,	 size(2)/2,     // Front-top-left
			-size(0)/2,	 size(1)/2,	-size(2)/2,    // Back-top-left
			-size(0)/2,	-size(1)/2,	 size(2)/2,    // Front-bottom-left
			-size(0)/2,	-size(1)/2,	-size(2)/2,   // Back-bottom-left
			 size(0)/2,	-size(1)/2,	-size(2)/2,    // Back-bottom-right
			-size(0)/2,	 size(1)/2,	-size(2)/2,    // Back-top-left
			 size(0)/2,	 size(1)/2,	-size(2)/2      // Back-top-right		};
		};
		mVBO.create(cubeStrip, GL_STATIC_DRAW);
	}

private:
	void prepareScope(Scope &scope) override
	{
		GLTransformedRenderFunction::prepareScope(scope);
		scope.Set(molecular::util::HashUtils::MakeHash("aVerPos"),
				  Attribute<Eigen::Vector3f>());
		scope.Set(molecular::util::HashUtils::MakeHash("uColor"),
				  Uniform<std::array<float, 4>>(std::array<float, 4>(
					  { mColor(0), mColor(1), mColor(2), mColor(3) })));
	}
	glwrapper::GLVertexArray mVAO;
	glwrapper::GLArrayBuffer mVBO;
	components::GLMolecularProgram mProgram;
	Eigen::Vector4f mColor{ 0.3, 0.4, 0.2, 0.5 };
};
} // namespace components
} // namespace dream

namespace dream
{
namespace renderer
{
class GLCubeObject : public GLTransformedObject
{
public:
	GLCubeObject(const std::string &name = "GLCubeObject")
		: GLTransformedObject(name)
	{
		auto rf = std::make_shared<components::GLCubeRenderFunction>();
		mRenderFunction =
			std::static_pointer_cast<components::GLMultipleCaleeRenderFunction>(
				rf);
	}
	void syncSelf() override
	{
		GLTransformedObject::syncSelf();
		auto cubeRenderFunction =
			std::static_pointer_cast<components::GLCubeRenderFunction>(
				mRenderFunction);
		if (mSizeUpdated)
		{
			cubeRenderFunction->updateCube(mSize);
			mSizeUpdated = false;
		}
	}

private:
	Eigen::Vector3f mSize{ 1, 1, 1 };
	bool mSizeUpdated{ true };
};
} // namespace renderer
} // namespace dream
