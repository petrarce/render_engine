#pragma once
#include <GLObject.hpp>

namespace dream
{
namespace glwrapper
{
class GLObjectBinder
{
public:
	GLObjectBinder(GLObject &object);
	~GLObjectBinder();

private:
	GLObject &mGlObject;
};

} // namespace glwrapper
} // namespace dream