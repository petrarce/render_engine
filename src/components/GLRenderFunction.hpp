#pragma once
#include "GLMolecularProgram.hpp"

namespace dream
{
namespace components
{

class GLRenderFunction
{
public:
	void draw(const Scope &parentScope = Scope())
	{
		assert(mInitialized && "Render function was not initialized");
		drawImpl(parentScope);
	}

	virtual void init()
	{
		mInitialized = true;
	}

protected:
	virtual void drawImpl(const Scope &parentScope) = 0;
	virtual void prepareScope(Scope &scope)
	{
	}

private:
	bool mInitialized{ false };
};

class GLSingleCaleeRenderFunction : public GLRenderFunction
{
public:
	GLSingleCaleeRenderFunction(
		std::shared_ptr<GLRenderFunction> calee = nullptr);
	void setCalee(std::shared_ptr<GLRenderFunction> calee);

protected:
	void drawImpl(const Scope &parentScope) override;

	std::shared_ptr<GLRenderFunction> mCalee;
};

class GLMultipleCaleeRenderFunction : public GLRenderFunction
{
public:
	using CaleePtr	 = std::shared_ptr<GLRenderFunction>;
	using CaleeArray = std::vector<CaleePtr>;
	GLMultipleCaleeRenderFunction(const CaleeArray &calees = {});
	virtual ~GLMultipleCaleeRenderFunction()
	{
	}

	void setCalees(const CaleeArray &calees);
	void addCalee(CaleePtr calee)
	{
		mCalees.push_back(calee);
	}
	void removeCalee(CaleePtr calee)
	{
		std::remove_if(mCalees.begin(), mCalees.end(),
					   [calee](CaleePtr item) { return item == calee; });
	}

	void init() override
	{
		GLRenderFunction::init();
	}

protected:
	void drawImpl(const Scope &parentScope) override;

	CaleeArray mCalees;
};
} // namespace components
} // namespace dream
