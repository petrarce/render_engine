#include <GLAssetManager.hpp>
#include <GLWrapperCore>
#include <GLMolecularProgram.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace dream
{
namespace renderer
{

template <>
std::shared_ptr<glwrapper::GLTexture2D>
GLAssetManager<glwrapper::GLTexture2D>::loadAsset(
	const std::string &path, glwrapper::GLTexture2D::InternalFormat format)
{
	std::cout << "load texture template" << std::endl;
	int width = 0, height = 0, components = 0;
	unsigned char *data =
		stbi_load(path.c_str(), &width, &height, &components, 0);
	if (!data)
	{
		std::cerr << "Failed to load texture from " << path.c_str()
				  << std::endl;
		return nullptr;
	}
	std::vector<char> dataBuf(width * height * components);
	std::copy(data, data + dataBuf.size(), dataBuf.begin());
	free(data);
	auto texture = std::make_shared<glwrapper::GLTexture2D>();
	glwrapper::GLTexture2D::DataFormat dataFormat;
	if (components == 1)
		dataFormat = glwrapper::GLTexture2D::DataFormat::DRed;
	else if (components == 2)
		dataFormat = glwrapper::GLTexture2D::DataFormat::DRg;
	else if (components == 3)
		dataFormat = glwrapper::GLTexture2D::DataFormat::DRgb;
	else if (components == 4)
		dataFormat = glwrapper::GLTexture2D::DataFormat::DRgba;
	else
	{
		assert(0 || "unexpected number of components");
		return nullptr;
	}
	std::array<size_t, 2> size;
	size[0] = width;
	size[1] = height;
	texture->create<char, 2>(dataBuf, 0, format, dataFormat,
							 glwrapper::GLTexture2D::DataType::UByte, size);
	texture->generateMipMaps();
	return texture;
}

template <>
std::shared_ptr<components::GLMolecularProgram>
GLAssetManager<components::GLMolecularProgram>::loadAsset(
	const components::Scope &scope)
{
	auto program = std::make_shared<components::GLMolecularProgram>();
	program->generate(scope);
	return program;
}

template <>
std::shared_ptr<GLMeshObject>
GLAssetManager<GLMeshObject>::loadAsset(const std::string &filePath)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(
		filePath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs |
							  aiProcess_FixInfacingNormals |
							  aiProcess_CalcTangentSpace);
	if (!scene)
	{
		std::cerr << "Failed to load mesh file " + filePath << std::endl;
		return nullptr;
	}
	if (scene->mNumMeshes > 1)
	{
		std::cout << "Multiple meshes are not supported" << std::endl;
		return nullptr;
	}
	if (scene->mNumMeshes == 0)
	{
		std::cerr << "No meshes available" << std::endl;
		return nullptr;
	}

	aiMesh *mesh = scene->mMeshes[0];
	assert(mesh->HasPositions() && mesh->HasFaces());

	auto meshObject = std::make_shared<GLMeshObject>();

	std::vector<float> vertexData;
	vertexData.reserve(mesh->mNumVertices * (3 + 3 * (mesh->HasNormals()) +
											 2 * (mesh->HasTextureCoords(0))));

	aiVector3D *vertices = mesh->mVertices;
	vertexData.insert(vertexData.end(), reinterpret_cast<float *>(vertices),
					  reinterpret_cast<float *>(&vertices[mesh->mNumVertices]));

	if (mesh->HasNormals())
	{
		meshObject->availableComponents.normals = true;
		aiVector3D *normals						= mesh->mNormals;
		vertexData.insert(
			vertexData.end(), reinterpret_cast<float *>(normals),
			reinterpret_cast<float *>(&normals[mesh->mNumVertices]));
	}

	if (mesh->HasTextureCoords(0))
	{
		meshObject->availableComponents.textureCoordinates = true;

		aiVector3D *textures = mesh->mTextureCoords[0];
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertexData.push_back(textures[i].x);
			vertexData.push_back(textures[i].y);
		}
	}
	if (mesh->HasTangentsAndBitangents())
	{
		meshObject->availableComponents.tangentspace = true;
		aiVector3D *tangents						 = mesh->mTangents;
		vertexData.insert(
			vertexData.end(), reinterpret_cast<float *>(tangents),
			reinterpret_cast<float *>(&tangents[mesh->mNumVertices]));
		aiVector3D *bitangents = mesh->mBitangents;
		vertexData.insert(
			vertexData.end(), reinterpret_cast<float *>(bitangents),
			reinterpret_cast<float *>(&bitangents[mesh->mNumVertices]));
	}

	std::vector<unsigned int> indexData;
	indexData.reserve(mesh->mNumFaces * 3);
	meshObject->availableComponents.indices = true;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		auto face = mesh->mFaces[i];
		if (face.mNumIndices != 3)
			throw std::runtime_error("Nontriangular faces are not supported");
		indexData.insert(indexData.end(), face.mIndices,
						 face.mIndices + face.mNumIndices);
	}

	meshObject->VAB.create(vertexData, GL_STATIC_DRAW);
	meshObject->EAB.create(indexData, GL_STATIC_DRAW);
	meshObject->numVertices = mesh->mNumVertices;
	meshObject->numIndices	= mesh->mNumFaces * 3;

	return meshObject;
}

GLMeshObject::GLMeshObject(const geometry::GLMesh &mesh)
{

	std::vector<float> vertexData;
	vertexData.reserve(mesh.vertices().size() *
					   (sizeof(mesh.vertices()[0]) +
						sizeof(mesh.normals()[0]) * mesh.hasNormals() +
						sizeof(mesh.texCoord()[0]) * mesh.hasTexCoord() +
						sizeof(mesh.tangents()[0]) +
						sizeof(mesh.bitangents()[0]) *
							(mesh.hasBitangents() && mesh.hasTangents()) +
						sizeof(mesh.colors()[0]) * mesh.hasColors()) /
					   sizeof(decltype(vertexData[0])));

	const auto &vertices = mesh.vertices();
	vertexData.insert(vertexData.end(),
					  reinterpret_cast<const float *>(vertices.begin().base()),
					  reinterpret_cast<const float *>(vertices.end().base()));

	if (mesh.hasNormals())
	{
		availableComponents.normals = true;
		const auto &normals			= mesh.normals();
		vertexData.insert(
			vertexData.end(),
			reinterpret_cast<const float *>(normals.begin().base()),
			reinterpret_cast<const float *>(normals.end().base()));
	}

	if (mesh.hasTexCoord())
	{
		availableComponents.textureCoordinates = true;

		const auto &textures = mesh.texCoord();
		vertexData.insert(
			vertexData.end(),
			reinterpret_cast<const float *>(textures.begin().base()),
			reinterpret_cast<const float *>(textures.end().base()));
	}
	if (mesh.hasBitangents() && mesh.hasTangents())
	{
		availableComponents.tangentspace = true;
		vertexData.insert(
			vertexData.end(),
			reinterpret_cast<const float *>(mesh.tangents().begin().base()),
			reinterpret_cast<const float *>(mesh.tangents().end().base()));
		vertexData.insert(
			vertexData.end(),
			reinterpret_cast<const float *>(mesh.bitangents().begin().base()),
			reinterpret_cast<const float *>(mesh.bitangents().end().base()));
	}
	if (mesh.hasColors())
	{
		availableComponents.colors = true;
		vertexData.insert(
			vertexData.end(),
			reinterpret_cast<const float *>(mesh.colors().begin().base()),
			reinterpret_cast<const float *>(mesh.colors().end().base()) + 4);
	}

	availableComponents.indices = mesh.hasIndices();

	VAB.create(vertexData, GL_STATIC_DRAW);
	EAB.create(mesh.indices(), GL_STATIC_DRAW);
	numVertices = mesh.vertices().size();
	numIndices	= mesh.indices().size();
}

} // namespace renderer
} // namespace dream
