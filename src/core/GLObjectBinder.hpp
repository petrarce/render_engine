#pragma once
#include <GLObject.hpp>

namespace dream
{
namespace glwrapper
{
class GlObjectBinder
{
public:
	GlObjectBinder(GLObject &object);
	~GlObjectBinder();

private:
	GLObject &mGlObject;
};

} // namespace glwrapper
} // namespace dream