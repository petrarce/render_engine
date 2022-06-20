#pragma once
#include <GLComponents>
#include <GLWrapperCore>

namespace dream
{
namespace components
{
class GLTriangleDrawer : public GLRenderFunction
{
public:
	GLTriangleDrawer();

protected:
	void drawImpl(const Scope &parentScope) override;

	glwrapper::GLVertexArray mVAO;
	glwrapper::GLArrayBuffer mVAB;
	GLMolecularProgram mProgram;
};

} // namespace components
} // namespace dream
