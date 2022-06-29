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

	const Eigen::Matrix4f &transform() const
	{
		return mTransform;
	}
	void setTransform(const Eigen::Matrix4f &transform)
	{
		mTransform = transform;
	}

	void init() override
	{
		GLMultipleCaleeRenderFunction::init();
	}

protected:
	void drawImpl(const Scope &parentScope) override
	{
		Scope scope(parentScope);
		prepareScope(scope);
		GLMultipleCaleeRenderFunction::drawImpl(scope);
	}

	void prepareScope(Scope &scope) override
	{
		using namespace molecular::util;

		if (scope.Has("uModel"_H))
		{
			auto transform = scope.Get<Uniform<Eigen::Matrix4f>>("uModel"_H);
			transform.value() *= mTransform;
			scope.Set<Uniform<Eigen::Matrix4f>>("uModel"_H, transform);
		}
		else
			scope.Set<Uniform<Eigen::Matrix4f>>("uModel"_H, mTransform);
	}
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
