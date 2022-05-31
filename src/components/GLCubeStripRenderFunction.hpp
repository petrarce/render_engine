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

	void draw(const dream::components::Scope &scp) override;

public:
	dream::glwrapper::GLArrayBuffer mVBO;
	dream::glwrapper::GLVertexArray mVAO;
	GLMolecularProgram mProg;
};
} // namespace components
} // namespace dream
