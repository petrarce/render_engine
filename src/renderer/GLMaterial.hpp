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
		using namespace molecular::util;
		mMeshBuffers = GLAssetManager<GLMeshObject>::getAsset(
			"GLMeshWithMaterialRenderFunctionMeshDefault"_H);
		if (mMeshBuffers == nullptr)
		{
			mMeshBuffers = GLAssetManager<GLMeshObject>::createAsset(
				"GLMeshWithMaterialRenderFunctionMeshDefault"_H);
			mMeshBuffers->VAB.create(
				std::vector<float>{ // vertex positions
									0, 0, 0, 5, 0, 10, 10, 0, 10,
									// texture coordinates
									0, 0, 0.5, 1, 1, 0 },
				GL_STATIC_DRAW);
			mMeshBuffers->availableComponents.textureCoordinates = true;
			mMeshBuffers->numVertices							 = 3;
		}

		resetVertexArrayObject();
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
		mMeshBuffers			  = std::make_shared<GLMeshObject>();
		mMeshBuffers->numVertices = vertices.size() / 3;
		mMeshBuffers->numIndices  = indices.has_value() ? indices->size() : 0;
		mMeshBuffers->availableComponents = { false, false, false };

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
			vertexAttributeBuffer.insert(vertexAttributeBuffer.end(),
										 textureCoordinates->begin(),
										 textureCoordinates->end());
			mMeshBuffers->availableComponents.textureCoordinates = true;
		}

		if (normals.has_value())
		{
			vertexAttributeBuffer.insert(vertexAttributeBuffer.end(),
										 normals->begin(), normals->end());
			mMeshBuffers->availableComponents.normals = true;
		}

		// deploy indices
		// TODO: implement

		// reallocate buffer on gpu
		mMeshBuffers->VAB.create(vertexAttributeBuffer, GL_STATIC_DRAW);
		resetVertexArrayObject();
	}

	void setMesh(const std::string &meshPath)
	{
		using namespace molecular::util;

		mMeshBuffers = GLAssetManager<GLMeshObject>::addAsset(meshPath);
		if (!mMeshBuffers)
			mMeshBuffers = GLAssetManager<GLMeshObject>::getAsset(
				"GLMeshWithMaterialRenderFunctionMeshDefault"_H);
		assert(mMeshBuffers != nullptr);
		resetVertexArrayObject();
	}

protected:
	void drawImpl(const components::Scope &parentScope) override
	{
		using namespace molecular::util;
		components::Scope scope(parentScope);
		prepareScope(scope);

		auto program =
			GLAssetManager<components::GLMolecularProgram>::addAsset(scope);
		program->prepare(scope);

		glwrapper::GLObjectBinder bindVAO(mVAO);
		glwrapper::GLObjectBinder bindProgram(*program);
		glwrapper::GLEnable<true, GL_BLEND> enableBlend;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDrawArrays(GL_TRIANGLES, 0, mMeshBuffers->numVertices);
		GLTransformedRenderFunction::drawImpl(parentScope);
	}

	void prepareScope(components::Scope &scope) override
	{
		using namespace molecular::util;
		GLTransformedRenderFunction::prepareScope(scope);
		scope.Set("aVerPos"_H, components::Attribute<Eigen::Vector3f>());

		const Texture *textureName = std::get_if<Texture>(&mAmbientColor);
		if (textureName && mMeshBuffers->availableComponents.textureCoordinates)
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

	void resetVertexArrayObject()
	{
		mVAO.createAttribute(verticesAttributeSpec, mMeshBuffers->VAB);

		if (mMeshBuffers->availableComponents.textureCoordinates)
		{
			auto texCoord	= textureAttributeSpec;
			texCoord.offset = verticesAttributeSpec.components *
							  mMeshBuffers->numVertices * sizeof(float);
			mVAO.createAttribute(texCoord, mMeshBuffers->VAB);
		}
		else
			mVAO.disableAttribute(textureAttributeSpec.location);

		if (mMeshBuffers->availableComponents.normals)
		{
			auto normals = normalsAttributeSpec;
			normals.offset =
				(verticesAttributeSpec.components +
				 textureAttributeSpec.components *
					 mMeshBuffers->availableComponents.textureCoordinates) *
				mMeshBuffers->numVertices * sizeof(float);
			mVAO.createAttribute(normals, mMeshBuffers->VAB);
		}
		else
			mVAO.disableAttribute(normalsAttributeSpec.location);
	}
	AmbientType mAmbientColor;

	std::shared_ptr<GLMeshObject> mMeshBuffers;

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
