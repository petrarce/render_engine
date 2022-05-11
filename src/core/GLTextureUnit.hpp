#pragma once
#include <glad/glad.h>
#include <GLObject.hpp>
#include <GLDefinitions.hpp>
#include <string>
#include <cassert>
namespace glwrapper
{
namespace core
{
class GlObjectBinder;

class GLTextureUnit : public GLObject
{
public:
	GLTextureUnit(GLenum unitNumber, const std::string &name = "")
		: GLObject(name)
	{

		mObjectId = unitNumber;
		GLuint maxUnitsNumber = 0;
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
					  reinterpret_cast<GLint *>(&maxUnitsNumber));
		assert(maxUnitsNumber + GL_TEXTURE0 > mObjectId &&
			   "Texture unit is out of range");
		assert(mObjectId >= GL_TEXTURE0 &&
			   "Invalid enumeration for texture unit");
	}
	virtual ~GLTextureUnit() = default;

protected:
	void bind() override
	{
		glGetIntegerv(GL_ACTIVE_TEXTURE, &mObjectBeforeBinding);
		glActiveTexture(mObjectId);
	}
	void unbind() override
	{
		glActiveTexture(mObjectBeforeBinding);
	}
};

} // namespace core
} // namespace glwrapper
