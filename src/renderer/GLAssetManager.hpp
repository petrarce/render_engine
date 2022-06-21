#pragma once
#include <memory>
#include <map>
#include <cassert>
#include <molecular/util/Hash.h>
#include <Singleton.hpp>
#include <GLWrapperCore>
#include <GLMolecularProgram.hpp>

namespace dream
{
namespace renderer
{

struct GLMeshObject
{
	struct Components
	{
		bool normals : 1;
		bool textureCoordinates : 1;
		bool indices : 1;
		bool tangentspace : 1;
	};
	Components availableComponents = { false, false, false, false };
	unsigned int numVertices	   = 0;
	unsigned int numIndices		   = 0;
	glwrapper::GLArrayBuffer VAB;
	glwrapper::GLElementArrayBuffer EAB;
};

template <class AssetT>
class GLAssetManager
	: public dream::components::Singleton<GLAssetManager<AssetT>>
{
public:
	using AssetPtr = std::shared_ptr<AssetT>;

	template <class ItentifierT, class... Args>
	static AssetPtr addAsset(const ItentifierT &assetIdentifier,
							 const Args &...args)
	{
		auto &manager = GLAssetManager<AssetT>::instance();
		auto id		  = molecular::util::HashUtils::MakeHash(assetIdentifier);
		if (manager.mAssets.find(id) == manager.mAssets.end())
		{
			auto asset = loadAsset(assetIdentifier, args...);
			if (!asset)
				return nullptr;
			manager.mAssets.insert({ id, asset });
		}
		return manager.mAssets.at(id);
	}

	static AssetPtr addAsset(molecular::util::Hash id, AssetPtr asset)
	{
		auto &manager = GLAssetManager<AssetT>::instance();
		if (manager.mAssets.find(id) == manager.mAssets.end())
			manager.mAssets.insert({ id, asset });
		else
			manager.mAssets.at(id) = asset;

		return asset;
	}

	static AssetPtr getAsset(molecular::util::Hash id)
	{
		auto &manager = GLAssetManager<AssetT>::instance();
		if (manager.mAssets.find(id) == manager.mAssets.end())
			return nullptr;
		return manager.mAssets.at(id);
	}

	static AssetPtr createAsset(molecular::util::Hash id)
	{
		auto &manager = GLAssetManager<AssetT>::instance();
		if (manager.mAssets.find(id) == manager.mAssets.end())
			manager.mAssets.insert({ id, std::make_shared<AssetT>() });

		return manager.mAssets.at(id);
	}

	static AssetPtr removeAsset(molecular::util::Hash id)
	{
		auto &manager = GLAssetManager<AssetT>::instance();
		if (manager.mAssets.find(id) == manager.mAssets.end())
			return nullptr;
		auto asset	  = manager.mAssets.id();
		size_t erased = manager.mAssets.erase(id);
		assert(erased == 1);
		return asset;
	}

private:
	static AssetPtr loadAsset(const std::string &assetPath);
	static AssetPtr loadAsset(const std::string &assetPath,
							  glwrapper::GLTexture2D::InternalFormat format);
	static AssetPtr loadAsset(const dream::components::Scope &scope);

	std::map<molecular::util::Hash, AssetPtr> mAssets;
};

} // namespace renderer
} // namespace dream
