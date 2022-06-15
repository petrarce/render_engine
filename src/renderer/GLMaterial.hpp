#pragma once
#include <GLWrapperCore>
#include <GLComponents>
#include <variant>
#include <string>
#include <array>
#include <optional>
#include "GLTransformedObject.hpp"
#include <GLAssetManager.hpp>
#include <molecular/util/Hash.h>

namespace dream
{
namespace renderer
{

class GLMeshWithMaterialRenderFunction
	: public components::GLTransformedRenderFunction
{
public:
	struct Texture
	{
		std::string path;
		glwrapper::GLTexture2D::InternalFormat internalFormat;
	};

	using AmbientType = std::variant<Texture, Eigen::Vector4f>;

	GLMeshWithMaterialRenderFunction()
		: components::GLTransformedRenderFunction()
	{
		mVertexAttributesBuffer.create(
			std::vector<float>{ // vertex positions
								0, 0, 0, 5, 0, 10, 10, 0, 10,
								// texture coordinates
								0, 0, 0.5, 1, 1, 0 },
			GL_STATIC_DRAW);
		mInstances = 3;
		mVAO.createAttribute(verticesAttributeSpec, mVertexAttributesBuffer);

		auto textureSpec   = textureAttributeSpec;
		textureSpec.offset = sizeof(float) * 9;
		mVAO.createAttribute(textureSpec, mVertexAttributesBuffer);
		mTextureEnabled = true;
	}

	void draw(const components::Scope &parentScope) override
	{
		using namespace molecular::util;
		components::Scope scope(parentScope);
		prepareScope(scope);

		auto program =
			GLAssetManager<components::GLMolecularProgram>::addAsset(scope);
		program->prepare(scope);

		glwrapper::GLObjectBinder bindVAO(mVAO);
		glwrapper::GLObjectBinder bindProgram(*program);

		glDrawArrays(GL_TRIANGLES, 0, mInstances);

		GLTransformedRenderFunction::draw(parentScope);
	}

	void setAmbientColor(const AmbientType &ambientColor)
	{
		mAmbientColor	 = std::variant(ambientColor);
		Texture *texture = std::get_if<Texture>(&mAmbientColor);
		if (texture)
		{
			auto glTexture = GLAssetManager<glwrapper::GLTexture2D>::addAsset(
				texture->path, texture->internalFormat);
			glTexture->setParameter(
				glwrapper::GLTexture2D::ValueMagFilter::MagLinear);
			glTexture->setParameter(
				glwrapper::GLTexture2D::ValueMinFilter::MinLinear);
			glTexture->setParameter(
				glwrapper::GLTexture2D::ValueWrapS::RepeatS);
			glTexture->setParameter(
				glwrapper::GLTexture2D::ValueWrapT::RepeatT);
		}
	}

	void setMesh(const std::vector<float> &vertices,
				 const std::optional<std::vector<float>> &normals,
				 const std::optional<std::vector<float>> &textureCoordinates,
				 const std::optional<std::vector<unsigned int>> &indices)
	{
		assert(vertices.size() % verticesAttributeSpec.components == 0);
		assert(!normals.has_value() ||
			   normals->size() % normalsAttributeSpec.components == 0);
		assert(!textureCoordinates.has_value() ||
			   textureCoordinates->size() % textureAttributeSpec.components ==
				   0);
		mInstances =
			(indices.has_value() ? indices->size() : vertices.size() / 3);
		size_t bufferSize =
			vertices.size() + (normals.has_value() ? normals->size() : 0) +
			(textureCoordinates.has_value() ? textureCoordinates->size() : 0);
		std::vector<float> vertexAttributeBuffer;
		vertexAttributeBuffer.reserve(bufferSize);

		// deploy vertex buffer
		vertexAttributeBuffer.insert(vertexAttributeBuffer.end(),
									 vertices.begin(), vertices.end());

		// deploy texture buffer
		if (textureCoordinates.has_value() &&
			(textureCoordinates->size() % 2) == (vertices.size() % 3))
		{
			auto textureSpecs	= textureAttributeSpec;
			textureSpecs.offset = sizeof(float) * vertexAttributeBuffer.size();
			vertexAttributeBuffer.insert(vertexAttributeBuffer.end(),
										 textureCoordinates->begin(),
										 textureCoordinates->end());
			mVAO.createAttribute(textureSpecs, mVertexAttributesBuffer);
			mTextureEnabled = true;
		}
		else
		{
			mVAO.disableAttribute(textureAttributeSpec.location);
			mTextureEnabled = false;
		}

		if (normals.has_value())
		{
			auto normalSpecs   = normalsAttributeSpec;
			normalSpecs.offset = sizeof(float) * vertexAttributeBuffer.size();
			vertexAttributeBuffer.insert(vertexAttributeBuffer.end(),
										 normals->begin(), normals->end());
			mVAO.createAttribute(normalSpecs, mVertexAttributesBuffer);
			mNormalsEnabled = true;
		}
		else
		{
			mVAO.disableAttribute(normalsAttributeSpec.location);
			mNormalsEnabled = false;
		}

		// deploy indices
		// TODO: implement
		mIndecisEnabled = false;

		// reallocate buffer on gpu
		mVertexAttributesBuffer.create(vertexAttributeBuffer, GL_STATIC_DRAW);
	}

protected:
	void prepareScope(components::Scope &scope) override
	{
		using namespace molecular::util;
		GLTransformedRenderFunction::prepareScope(scope);
		scope.Set("aVerPos"_H, components::Attribute<Eigen::Vector3f>());

		const Texture *textureName = std::get_if<Texture>(&mAmbientColor);
		if (textureName && mTextureEnabled)
		{
			auto texture = GLAssetManager<glwrapper::GLTexture2D>::getAsset(
				HashUtils::MakeHash(textureName->path));
			if (texture)
			{
				texture->attach(glwrapper::GLTextureUnit::Texture0 + 1);
				scope.Set("uAmbiantTexture"_H, components::Uniform<int>(1));
				scope.Set("aTextureCoord"_H,
						  components::Attribute<Eigen::Vector2f>());
			}
			else
			{
				std::array<float, 4> clr = {
					mDefaultColor(0),
					mDefaultColor(1),
					mDefaultColor(2),
					mDefaultColor(3),
				};
				scope.Set("uAmbiantColor"_H,
						  components::Uniform<typeof clr>(clr));
			}
		}
		else
		{
			const Eigen::Vector4f *color =
				std::get_if<Eigen::Vector4f>(&mAmbientColor);
			if (color)
			{
				std::array<float, 4> clr;
				clr = { color->operator()(0), color->operator()(1),
						color->operator()(2), color->operator()(3) };
				scope.Set("uAmbiantColor"_H,
						  components::Uniform<typeof clr>(clr));
			}
			else
			{
				std::array<float, 4> clr = {
					mDefaultColor(0),
					mDefaultColor(1),
					mDefaultColor(2),
					mDefaultColor(3),
				};
				scope.Set("uAmbiantColor"_H,
						  components::Uniform<typeof clr>(clr));
			}
		}
	}
	AmbientType mAmbientColor;
	unsigned int mInstances{ 0 };

	glwrapper::GLArrayBuffer mVertexAttributesBuffer;
	bool mNormalsEnabled{ false };
	bool mTextureEnabled{ false };

	glwrapper::GLElementArrayBuffer mIndexBuffer;
	bool mIndecisEnabled{ false };

	glwrapper::GLVertexArray mVAO;

	static const Eigen::Vector4f mDefaultColor;
	static const glwrapper::GLVertexArray::AttributeSpecification
		verticesAttributeSpec;
	static const glwrapper::GLVertexArray::AttributeSpecification
		textureAttributeSpec;
	static const glwrapper::GLVertexArray::AttributeSpecification
		normalsAttributeSpec;
};

} // namespace renderer
} // namespace dream
namespace dream
{
namespace renderer
{
class GLMeshWithMaterialObject : public GLTransformedObject
{
public:
	struct Mesh
	{
		std::optional<std::vector<unsigned int>> indices{ std::nullopt };
		std::optional<std::vector<float>> normals{ std::nullopt };
		std::optional<std::vector<float>> textures{ std::nullopt };
		std::vector<float> vertices;
	};

	using AmbientData = GLMeshWithMaterialRenderFunction::AmbientType;

	GLMeshWithMaterialObject(const std::string &name = "GLMeshWithMaterial")
		: GLTransformedObject(name)
		, mAmbient(Eigen::Vector4f(1, 1, 1, 1))
		, mMesh(std::make_shared<Mesh>(Mesh{
			  .indices	= std::nullopt,
			  .normals	= std::nullopt,
			  .textures = std::vector<float>({ 0, 0, 0.5, 1, 1, 0 }),
			  .vertices = std::vector<float>({ 0, 0, 0, 5, 0, 10, 10, 0, 10 }),
		  }))
	{
		mRenderFunction = std::make_shared<GLMeshWithMaterialRenderFunction>();
	}
	void setAmbient(const AmbientData &ambient)
	{
		mAmbient		= ambient;
		mAmbientChanged = true;
	}

	const AmbientData &ambient() const
	{
		return mAmbient;
	}

	void setMesh(const std::shared_ptr<Mesh> mesh)
	{
		mMesh		 = mesh;
		mMeshChanged = true;
	}

	void syncSelf() override
	{
		GLTransformedObject::syncSelf();
		auto rf = std::static_pointer_cast<GLMeshWithMaterialRenderFunction>(
			mRenderFunction);
		if (mAmbientChanged)
		{
			rf->setAmbientColor(mAmbient);
			mAmbientChanged = false;
		}
		if (mMeshChanged && mMesh)
		{
			mMeshChanged = false;
			rf->setMesh(mMesh->vertices, mMesh->normals, mMesh->textures,
						mMesh->indices);
		}
	}

private:
	AmbientData mAmbient;
	bool mAmbientChanged{ true };
	std::shared_ptr<Mesh> mMesh;
	bool mMeshChanged{ false };
};
} // namespace renderer
} // namespace dream
