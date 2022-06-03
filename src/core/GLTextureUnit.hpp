#pragma once
#include <glad/glad.h>
#include <GLObject.hpp>
#include <GLDefinitions.hpp>
#include <string>
#include <cassert>
namespace dream
{
namespace glwrapper
{
class GLObjectBinder;

class GLTextureUnit : public GLObject
{
public:
	enum TextureUnit
	{
		Texture0 = GL_TEXTURE0,
	};
	GLTextureUnit(GLenum unitNumber, const std::string &name = "")
		: GLObject(name)
	{

		mObjectId			  = unitNumber;
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
		mObjectsBeforeBinding.push_back(0);
		glGetIntegerv(GL_ACTIVE_TEXTURE, &mObjectsBeforeBinding.back());
		glActiveTexture(mObjectId);
	}
	void unbind() override
	{
		glActiveTexture(mObjectsBeforeBinding.back());
		mObjectsBeforeBinding.pop_back();
	}
};

} // namespace glwrapper
} // namespace dream
