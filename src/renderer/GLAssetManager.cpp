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
		return nullptr;
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
							  aiProcess_FixInfacingNormals);
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
	vertexData.insert(
		vertexData.end(), reinterpret_cast<float *>(vertices),
		reinterpret_cast<float *>(&vertices[mesh->mNumVertices - 1]) + 3);

	if (mesh->HasNormals())
	{
		meshObject->availableComponents.normals = true;
		aiVector3D *normals						= mesh->mNormals;
		vertexData.insert(
			vertexData.end(), reinterpret_cast<float *>(normals),
			reinterpret_cast<float *>(&normals[mesh->mNumVertices - 1]) + 3);
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

} // namespace renderer
} // namespace dream
