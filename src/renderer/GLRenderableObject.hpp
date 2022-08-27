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
		mRenderFunction =
			std::make_shared<components::GLMultipleCaleeRenderFunction>();
	}
	~GLRenderableObject()
	{
		clearChildren();
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

	void sync()
	{
		if (mSynchronized)
			return;
		if (!mRenderFunction->isInitialized())
			mRenderFunction->init();
		syncSelf();
		mSynchronized = true;
		for (auto c : mChildren) c->sync();
	}

	void resetSync()
	{
		if (!mSynchronized)
			return;
		mSynchronized = false;
		for (auto c : mChildren) c->resetSync();
	}

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
	template <class T>
	void setUniform(const std::string &name, const T &value)
	{
		mRenderFunction->setUniform(name, value);
	}

	void unsetUniform(const std::string &name)
	{
		mRenderFunction->unsetUniform(name);
	}

protected:
	virtual void syncSelf(){};

	std::string mName;
	std::vector<std::shared_ptr<GLRenderableObject>> mChildren;
	std::shared_ptr<components::GLMultipleCaleeRenderFunction> mRenderFunction;
	bool mSynchronized{ false };
};

} // namespace renderer
} // namespace dream
