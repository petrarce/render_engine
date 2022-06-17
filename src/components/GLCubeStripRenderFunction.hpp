#pragma once
#include <GLWrapperCore>
#include "GLRenderFunction.hpp"
#include "GLMolecularProgram.hpp"
namespace dream
{
namespace components
{

class GLCubeStripRenderFunction : public GLRenderFunction
{
public:
	GLCubeStripRenderFunction();

	dream::glwrapper::GLArrayBuffer mVBO;
	dream::glwrapper::GLVertexArray mVAO;
	GLMolecularProgram mProg;

protected:
	void drawImpl(const dream::components::Scope &scp) override;
};
} // namespace components
} // namespace dream
