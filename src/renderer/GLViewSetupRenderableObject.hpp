#pragma once
#include <GLRenderableObject.hpp>
#include <GLComponents>
#include <GLWrapperCore>
#include <Camera.hpp>
#include <molecular/util/Matrix4.h>

namespace dream
{
namespace components
{
class GLViewSetupRenderFunction : public GLMultipleCaleeRenderFunction
{
public:
	GLViewSetupRenderFunction()
		: GLMultipleCaleeRenderFunction()
	{
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

protected:
	void prepareScope(components::Scope &scope) override
	{
		using namespace molecular::util;
		scope.Set(
			"uViewDirection"_H,
			Uniform<Eigen::Vector3f>(mViewMatrix.transpose().block<3, 3>(0, 0) *
									 Eigen::Vector3f(0, 0, -1)));
	}

	void drawImpl(const Scope &parentScope) override
	{
		Scope scope(parentScope);
		prepareScope(scope);
		Scope viewScope(scope);
		viewScope.Set(molecular::util::HashUtils::MakeHash("uView"),
					  Uniform<Eigen::Matrix4f>(mViewMatrix));
		viewScope.Set(molecular::util::HashUtils::MakeHash("uProjection"),
					  Uniform<Eigen::Matrix4f>(mProjectionMatrix));
		GLMultipleCaleeRenderFunction::drawImpl(viewScope);
	}

	Eigen::Matrix4f mViewMatrix;
	Eigen::Matrix4f mProjectionMatrix;
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

	void setCamera(const dream::geometry::GLCamera &camera)
	{
		mCamera		   = camera;
		mCameraUpdated = true;
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

private:
	void syncSelf() override
	{
		auto viewRenderFunction =
			std::static_pointer_cast<components::GLViewSetupRenderFunction>(
				mRenderFunction);
		if (mCameraUpdated)
		{
			viewRenderFunction->setViewMatrix(mCamera.toViewTransform());
			mCameraUpdated = false;
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
	}
	dream::geometry::GLCamera mCamera;
	bool mCameraUpdated = true;
	float mFar{ 1000.f };
	bool mFarChanged{ true };
	float mNear{ 0.1f };
	bool mNearChanged{ true };
	float mAspectRatio{ 2 };
	bool mAspectRatioChanged{ true };
	float mFOV{ 45.f };
	bool mFOVChanged{ true };
};
} // namespace renderer
} // namespace dream
