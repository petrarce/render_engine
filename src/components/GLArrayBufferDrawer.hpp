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

	void draw(const Scope &parentScope) override;

private:
	glwrapper::GLVertexArray mVAO;
	glwrapper::GLArrayBuffer mVAB;
	GLMolecularProgram mProgram;
};

} // namespace components
} // namespace dream
