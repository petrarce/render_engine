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

	enum RenderMode
	{
		Faces  = 0x1,
		Lines  = 0x2,
		Points = 0x4,
	};

	using AmbientType = std::variant<Texture, Eigen::Vector4f>;

	GLMeshWithMaterialRenderFunction()
		: components::GLTransformedRenderFunction()
	{
	}

	void setMap(const std::string &mapName, const Texture &map)
	{
		mMaps[molecular::util::HashUtils::MakeHash(mapName)] = map;

		auto texture = GLAssetManager<glwrapper::GLTexture2D>::addAsset(
			map.path, map.internalFormat);
		if (texture)
		{
			texture->setParameter(
				glwrapper::GLTexture2D::ValueMagFilter::MagLinear);
			texture->setParameter(
				glwrapper::GLTexture2D::ValueMinFilter::MinLinearMipmapLinear);
			texture->setParameter(glwrapper::GLTexture2D::ValueWrapS::RepeatS);
			texture->setParameter(glwrapper::GLTexture2D::ValueWrapT::RepeatT);
			texture->generateMipMaps();
		}
	}

	void removeMap(const std::string &mapName)
	{
		const auto it =
			mMaps.find(molecular::util::HashUtils::MakeHash(mapName));
		if (it != mMaps.end())
			mMaps.erase(it);
	}

	void setAmbientColor(const AmbientType &ambientColor)
	{
		mAmbientColor	 = std::variant(ambientColor);
		Texture *texture = std::get_if<Texture>(&mAmbientColor);
		if (texture)
		{
			auto glTexture = GLAssetManager<glwrapper::GLTexture2D>::addAsset(
				texture->path, texture->internalFormat);
			if (glTexture)
			{
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
	}

	void setMesh(const geometry::GLMesh &mesh)
	{

		mMeshBuffers = std::make_shared<renderer::GLMeshObject>(mesh);
		resetVertexArrayObject();
	}

	void setMesh(const std::string &meshPath)
	{
		using namespace molecular::util;

		mMeshBuffers = GLAssetManager<GLMeshObject>::addAsset(meshPath);
		if (!mMeshBuffers)
			return;
		assert(mMeshBuffers != nullptr);
		resetVertexArrayObject();
	}

	void init() override
	{
		GLTransformedRenderFunction::init();

		mVAO = std::make_unique<glwrapper::GLVertexArray>();
	}

	void setPointSize(float ps)
	{
		mPointSize = ps;
	}

	void setLineSize(float ls)
	{
		mLineWidth = ls;
	}

	void setRenderMode(int mode)
	{
		mRenderModes = mode;
	}

protected:
	void drawImpl(const components::Scope &parentScope) override
	{
		// first prepare scope for local object and draw them
		if (mMeshBuffers)
		{
			using namespace molecular::util;
			components::Scope scope(parentScope);
			GLMeshWithMaterialRenderFunction::prepareScope(scope);

			auto program =
				GLAssetManager<components::GLMolecularProgram>::addAsset(scope);
			program->prepare(scope);

			glwrapper::GLObjectBinder bindVAO(*mVAO);
			glwrapper::GLObjectBinder bindProgram(*program);
			glwrapper::GLEnable<true, GL_BLEND> enableBlend;

			glPointSize(mPointSize);
			glLineWidth(mLineWidth);
			if (mMeshBuffers->availableComponents.indices)
			{
				if (mRenderModes & RenderMode::Faces)
					glDrawElements(GL_TRIANGLES, mMeshBuffers->numIndices,
								   GL_UNSIGNED_INT, 0);

				if (mRenderModes & RenderMode::Lines)
					glDrawElements(GL_LINES, mMeshBuffers->numIndices,
								   GL_UNSIGNED_INT, 0);

				if (mRenderModes & RenderMode::Points)
					glDrawElements(GL_POINTS, mMeshBuffers->numIndices,
								   GL_UNSIGNED_INT, 0);
			}
			else
			{
				if (mRenderModes & RenderMode::Faces)
					glDrawArrays(GL_TRIANGLES, 0, mMeshBuffers->numVertices);

				if (mRenderModes & RenderMode::Lines)
					glDrawArrays(GL_LINES, 0, mMeshBuffers->numVertices);

				if (mRenderModes & RenderMode::Points)
					glDrawArrays(GL_POINTS, 0, mMeshBuffers->numVertices);
			}
		}

		// now draw children
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
				scope.Set("uDiffuseTexture"_H, components::Uniform<int>(1));
			}
			else
			{
				std::array<float, 4> clr = {
					mDefaultColor(0),
					mDefaultColor(1),
					mDefaultColor(2),
					mDefaultColor(3),
				};
				scope.Set("uDiffuseColor"_H,
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
				scope.Set("uDiffuseColor"_H,
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
				scope.Set("uDiffuseColor"_H,
						  components::Uniform<typeof clr>(clr));
			}
		}

		if (mMeshBuffers->availableComponents.normals)
			scope.Set("aNormal"_H, components::Attribute<Eigen::Vector3f>());
		if (mMeshBuffers->availableComponents.tangentspace)
		{
			scope.Set("aTangent"_H, components::Attribute<Eigen::Vector3f>());
			scope.Set("aBitangent"_H, components::Attribute<Eigen::Vector3f>());
		}
		if (mMeshBuffers->availableComponents.textureCoordinates)
			scope.Set("aTextureCoord"_H,
					  components::Attribute<Eigen::Vector2f>());

		int mapNum = 0;
		for (const auto &map : mMaps)
		{
			auto nmTexture = GLAssetManager<glwrapper::GLTexture2D>::getAsset(
				molecular::util::HashUtils::MakeHash(map.second.path));
			if (nmTexture)
			{
				scope.Set(map.first, components::Uniform<int>(2 + mapNum));
				nmTexture->attach(glwrapper::GLTextureUnit::Texture0 + 2 +
								  mapNum);
				mapNum++;
			}
		}

		scope.Set("uSpecularity"_H, components::Uniform<float>(16));
	}

	void resetVertexArrayObject()
	{
		mVAO->createAttribute(verticesAttributeSpec, mMeshBuffers->VAB,
							  mMeshBuffers->EAB);

		if (mMeshBuffers->availableComponents.normals)
		{
			auto normals   = normalsAttributeSpec;
			normals.offset = verticesAttributeSpec.components *
							 mMeshBuffers->numVertices * sizeof(float);
			mVAO->createAttribute(normals, mMeshBuffers->VAB,
								  mMeshBuffers->EAB);
		}
		else
			mVAO->disableAttribute(normalsAttributeSpec.location);

		if (mMeshBuffers->availableComponents.textureCoordinates)
		{
			auto texCoord	= textureAttributeSpec;
			texCoord.offset = (verticesAttributeSpec.components +
							   normalsAttributeSpec.components *
								   mMeshBuffers->availableComponents.normals) *
							  mMeshBuffers->numVertices * sizeof(float);
			mVAO->createAttribute(texCoord, mMeshBuffers->VAB,
								  mMeshBuffers->EAB);
		}
		else
			mVAO->disableAttribute(textureAttributeSpec.location);

		if (mMeshBuffers->availableComponents.tangentspace)
		{
			auto tangentsSpec = tangentAttributSpec;
			tangentsSpec.offset =
				(verticesAttributeSpec.components +
				 normalsAttributeSpec.components *
					 mMeshBuffers->availableComponents.normals +
				 textureAttributeSpec.components *
					 mMeshBuffers->availableComponents.textureCoordinates) *
				mMeshBuffers->numVertices * sizeof(float);
			mVAO->createAttribute(tangentsSpec, mMeshBuffers->VAB,
								  mMeshBuffers->EAB);
			auto bitangentSpec = bitangentAttributSpec;
			bitangentSpec.offset =
				(verticesAttributeSpec.components +
				 normalsAttributeSpec.components *
					 mMeshBuffers->availableComponents.normals +
				 textureAttributeSpec.components *
					 mMeshBuffers->availableComponents.textureCoordinates +
				 tangentAttributSpec.components) *
				mMeshBuffers->numVertices * sizeof(float);
			mVAO->createAttribute(bitangentSpec, mMeshBuffers->VAB,
								  mMeshBuffers->EAB);
		}
		else
		{
			mVAO->disableAttribute(tangentAttributSpec.location);
			mVAO->disableAttribute(bitangentAttributSpec.location);
		}
	}
	AmbientType mAmbientColor;

	std::map<molecular::util::Hash, Texture> mMaps;

	std::shared_ptr<GLMeshObject> mMeshBuffers;

	std::unique_ptr<glwrapper::GLVertexArray> mVAO;

	int mRenderModes{ RenderMode::Points };

	float mLineWidth{ 1 };
	float mPointSize{ 3 };

	static const Eigen::Vector4f mDefaultColor;
	static const glwrapper::GLVertexArray::AttributeSpecification
		verticesAttributeSpec;
	static const glwrapper::GLVertexArray::AttributeSpecification
		textureAttributeSpec;
	static const glwrapper::GLVertexArray::AttributeSpecification
		normalsAttributeSpec;
	static const glwrapper::GLVertexArray::AttributeSpecification
		tangentAttributSpec;
	static const glwrapper::GLVertexArray::AttributeSpecification
		bitangentAttributSpec;
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
	using AmbientData = GLMeshWithMaterialRenderFunction::AmbientType;
	using MeshVariant =
		std::variant<std::string, std::shared_ptr<geometry::GLMesh>>;
	using RenderMode = GLMeshWithMaterialRenderFunction::RenderMode;
	using Texture	 = GLMeshWithMaterialRenderFunction::Texture;
	GLMeshWithMaterialObject(const std::string &name = "GLMeshWithMaterial")
		: GLTransformedObject(name)
		, mMesh(std::shared_ptr<geometry::GLMesh>(nullptr))
	{
		mRenderFunction = std::make_shared<GLMeshWithMaterialRenderFunction>();
	}

	void setMesh(const MeshVariant mesh)
	{
		mMesh		 = mesh;
		mMeshChanged = true;
	}

	void setRenderMode(const int mode)
	{
		mRenderModes	   = mode;
		mRenderModeChanged = true;
	}

	void setMap(const std::string &name, const Texture map)
	{
		mMaps.insert({ name, map });
	}

	void syncSelf() override
	{
		GLTransformedObject::syncSelf();
		auto rf = std::static_pointer_cast<GLMeshWithMaterialRenderFunction>(
			mRenderFunction);

		if (mMeshChanged)
		{
			mMeshChanged = false;
			if (auto mesh =
					std::get_if<std::shared_ptr<geometry::GLMesh>>(&mMesh))
				rf->setMesh(*(mesh->get()));
			else if (auto mesh = std::get_if<std::string>(&mMesh))
				rf->setMesh(*mesh);
		}
		if (mRenderModeChanged)
		{
			rf->setRenderMode(mRenderModes);
			mRenderModeChanged = false;
		}
		if (!mMaps.empty())
		{
			for (const auto &map : mMaps) rf->setMap(map.first, map.second);
			mMaps.clear();
		}
	}

private:
	MeshVariant mMesh;
	bool mMeshChanged{ false }; // don't draw empty mesh by default
	int mRenderModes{ RenderMode::Faces };
	bool mRenderModeChanged{ true };
	std::map<std::string, Texture> mMaps;
};
} // namespace renderer
} // namespace dream
