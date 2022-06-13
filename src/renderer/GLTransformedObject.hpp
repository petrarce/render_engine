#pragma once
#include <GLRenderableObject.hpp>
#include <GLComponents>
#include <molecular/util/Hash.h>

namespace dream
{

namespace components
{

class GLTransformedRenderFunction : public GLMultipleCaleeRenderFunction
{
public:
	GLTransformedRenderFunction()
		: GLMultipleCaleeRenderFunction()
	{
	}
	virtual ~GLTransformedRenderFunction()
	{
	}

	void draw(const Scope &parentScope) override
	{
		using namespace molecular::util;
		Scope scope(parentScope);
		if (scope.Has("uModel"_H))
		{
			auto transform = scope.Get<Uniform<Eigen::Matrix4f>>("uModel"_H);
			transform.value() *= mTransform;
			scope.Set<Uniform<Eigen::Matrix4f>>("uModel"_H, transform);
		}
		else
			scope.Set<Uniform<Eigen::Matrix4f>>("uModel"_H, mTransform);

		drawSelf(scope);
		for (auto c : mCalees) c->draw(scope);
	}

	const Eigen::Matrix4f &transform() const
	{
		return mTransform;
	}
	void setTransform(const Eigen::Matrix4f &transform)
	{
		mTransform = transform;
	}

protected:
	virtual void drawSelf(const Scope &currentScope){};
	Eigen::Matrix4f mTransform{ Eigen::Matrix4f::Identity() };
};
} // namespace components
} // namespace dream

namespace dream
{

namespace renderer
{

class GLTransformedObject : public GLRenderableObject
{
public:
	GLTransformedObject(const std::string &name = "GLTransformedObject")
		: GLRenderableObject(name)
	{
		auto rf =
			std::make_shared<dream::components::GLTransformedRenderFunction>();
		mRenderFunction = std::static_pointer_cast<
			dream::components::GLMultipleCaleeRenderFunction>(rf);
	}
	const Eigen::Matrix4f &transform() const
	{
		return mTransform;
	}
	void setTransform(const Eigen::Matrix4f &transform)
	{
		mTransform = transform;
	}
	virtual ~GLTransformedObject()
	{
	}

protected:
	void syncSelf() override
	{
		auto rf = std::static_pointer_cast<
			dream::components::GLTransformedRenderFunction>(mRenderFunction);
		assert(rf);
		rf->setTransform(mTransform);
	}
	Eigen::Matrix4f mTransform{ Eigen::Matrix4f::Identity() };
};

} // namespace renderer
} // namespace dream
