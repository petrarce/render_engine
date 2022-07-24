#pragma once
#include <GLRenderableObject.hpp>
#include <GLComponents>
#include <GLWrapperCore>
#include <molecular/util/Matrix4.h>
#include <Light.hpp>

namespace dream
{
namespace components
{

class GLViewSetupRenderFunction : public GLMultipleCaleeRenderFunction
{
public:
	using LightPtr = std::shared_ptr<dream::geometry::Light>;

	GLViewSetupRenderFunction()
		: GLMultipleCaleeRenderFunction()
	{
		//		auto light = std::make_shared<dream::geometry::PointLight>();
		//		light->setColor({1, 1, 1});
		//		light->setPosition({10, 10, 10});
		//		mLights.push_back(light);
	}

	virtual ~GLViewSetupRenderFunction()
	{
	}

	void setViewMatrix(Eigen::Matrix4f view)
	{
		mViewMatrix = view;
	}

	void setProjectionMatrix(Eigen::Matrix4f projection)
	{
		mProjectionMatrix = projection;
	}

	void setLights(const std::vector<LightPtr> lights)
	{
		mLights = lights;
	}

protected:
	void prepareScope(components::Scope &scope) override
	{
		using namespace molecular::util;
		scope.Set("uViewPosition"_H,
				  Uniform<Eigen::Vector3f>(
					  (mViewMatrix.inverse() * Eigen::Vector4f(0, 0, 0, 1))
						  .block<3, 1>(0, 0)));
		scope.Set("uView"_H, Uniform<Eigen::Matrix4f>(mViewMatrix));
		scope.Set("uProjection"_H, Uniform<Eigen::Matrix4f>(mProjectionMatrix));
	}

	void drawImpl(const Scope &parentScope) override
	{
		Scope scope(parentScope);
		prepareScope(scope);
		for (auto light : mLights)
		{
			Scope lightScope(scope);
			prepareLightScope(lightScope, light);
			GLMultipleCaleeRenderFunction::drawImpl(lightScope);
		}
	}

	void prepareLightScope(components::Scope &lightScope, LightPtr light)
	{
		using namespace molecular::util;
		lightScope.Set("uLightColor"_H,
					   Uniform<Eigen::Vector3f>(light->color()));
		if (auto concreteLight =
				dynamic_cast<dream::geometry::DirectionLight *>(light.get()))
		{
			lightScope.Set(
				"uLightDirection"_H,
				Uniform<Eigen::Vector3f>(concreteLight->direction()));
		}
		else if (auto concreteLight =
					 dynamic_cast<dream::geometry::PointLight *>(light.get()))
		{
			lightScope.Set("uLightPosition"_H,
						   Uniform<Eigen::Vector3f>(concreteLight->position()));
		}
	}

	Eigen::Matrix4f mViewMatrix;
	Eigen::Matrix4f mProjectionMatrix;
	std::vector<LightPtr> mLights;
};
} // namespace components
} // namespace dream

namespace dream
{
namespace renderer
{
class GLViewSetupRenderableObject : public GLRenderableObject
{
public:
	using LightsVec =
		std::vector<dream::components::GLViewSetupRenderFunction::LightPtr>;
	GLViewSetupRenderableObject(
		const std::string &name = "GLViewSetupRenderableObject")
		: GLRenderableObject(name)
	{
		mRenderFunction =
			std::make_shared<components::GLViewSetupRenderFunction>();
	}

	~GLViewSetupRenderableObject()
	{
	}

	void setViewTransform(const Eigen::Matrix4f &viewTransofrm)
	{
		mViewTransofrm		  = viewTransofrm;
		mViewTransofrmUpdated = true;
	}

	void setFarPlane(float farPlane)
	{
		mFar		= farPlane;
		mFarChanged = true;
	}

	void setNearPlane(float nearPlane)
	{
		mNear		 = nearPlane;
		mNearChanged = true;
	}

	void setAspectRatio(float aspectRatio)
	{
		mAspectRatio		= aspectRatio;
		mAspectRatioChanged = true;
	}

	void setFov(float fov)
	{
		mFOV		= fov;
		mFOVChanged = true;
	}

	void setLights(const LightsVec lights)
	{
		mLights		   = lights;
		mLightsChanged = true;
	}

private:
	void syncSelf() override
	{
		auto viewRenderFunction =
			std::static_pointer_cast<components::GLViewSetupRenderFunction>(
				mRenderFunction);
		if (mViewTransofrmUpdated)
		{
			viewRenderFunction->setViewMatrix(mViewTransofrm);
			mViewTransofrmUpdated = false;
		}
		if (mFarChanged || mNearChanged || mAspectRatioChanged || mFOVChanged)
		{
			viewRenderFunction->setProjectionMatrix(
				molecular::util::Matrix4::ProjectionPerspective(
					mFOV, mAspectRatio, mNear, mFar)
					.ToEigen());
			mFarChanged			= false;
			mNearChanged		= false;
			mAspectRatioChanged = false;
			mFOVChanged			= false;
		}

		if (mLightsChanged)
		{
			viewRenderFunction->setLights(mLights);
			mLightsChanged = false;
		}
	}
	Eigen::Matrix4f mViewTransofrm{ Eigen::Matrix4f::Identity() };
	bool mViewTransofrmUpdated = true;
	float mFar{ 1000.f };
	bool mFarChanged{ true };
	float mNear{ 0.1f };
	bool mNearChanged{ true };
	float mAspectRatio{ 2 };
	bool mAspectRatioChanged{ true };
	float mFOV{ 45.f };
	bool mFOVChanged{ true };
	LightsVec mLights;
	bool mLightsChanged{ true };
};
} // namespace renderer
} // namespace dream
