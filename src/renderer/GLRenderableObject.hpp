#pragma once
#include <GLWrapperCore>
#include <GLComponents>

namespace dream
{
namespace renderer
{

class GLRenderableObject : public molecular::util::NonCopyable
{
public:
	GLRenderableObject(const std::string &name = "GLRenderableObject")
		: mName(name)
	{
	}
	~GLRenderableObject()
	{
		std::cout << mName + " about to be destructed" << std::endl;
		clearChildren();
		std::cout << mName + " destructed" << std::endl;
	}

	const std::vector<std::shared_ptr<GLRenderableObject>> &children() const
	{
		return mChildren;
	}
	std::vector<std::shared_ptr<GLRenderableObject>> &children()
	{
		return mChildren;
	}
	std::shared_ptr<components::GLMultipleCaleeRenderFunction> renderFunction()
	{
		return mRenderFunction;
	}

	void synchronize()
	{
		synchroniseSelf();
		for (auto c : mChildren) c->synchronize();
	}

	virtual void synchroniseSelf(){};

	void addChild(std::shared_ptr<GLRenderableObject> child)
	{
		assert(child != nullptr);
		mChildren.push_back(child);
		mRenderFunction->addCalee(child->renderFunction());
	}

	void removeChild(std::shared_ptr<GLRenderableObject> child)
	{
		mRenderFunction->removeCalee(child->renderFunction());
		mChildren.erase(
			std::remove_if(mChildren.begin(), mChildren.end(),
						   [child](std::shared_ptr<GLRenderableObject> item)
						   {
							   assert(item != nullptr);
							   return item == child;
						   }),
			mChildren.end());
	}

	void clearChildren()
	{
		while (!mChildren.empty())
		{
			assert(mChildren.back());
			removeChild(mChildren.back());
		}
	}

protected:
	std::string mName;
	std::vector<std::shared_ptr<GLRenderableObject>> mChildren;
	std::shared_ptr<components::GLMultipleCaleeRenderFunction> mRenderFunction;
};

} // namespace renderer
} // namespace dream
