#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>		// Output data structure
#include <assimp/postprocess.h> // Post processing flags

template <class IteratorBegin, class IteratorEnd>
void printData(const IteratorBegin &begin, const IteratorEnd &end,
			   int components)
{
	int cnt = 0;
	std::string item;
	for (auto iter = begin; iter != end; iter++, cnt++)
	{
		item += std::to_string(*iter);
		if (cnt % components == components - 1)
		{
			std::cout << "{ " + item + "}" << std::endl;
			item = "";
		}
		else
			item += ", ";
	}
}
int main(int argc, char **argv)
{
	Assimp::Importer importer;
	const aiScene *scene =
		importer.ReadFile(argv[1], aiProcess_Triangulate | aiProcess_FlipUVs |
									   aiProcess_FixInfacingNormals);
	if (!scene)
		throw std::runtime_error("Failed to load mesh file " +
								 std::string(argv[1]));
	if (scene->mNumMeshes > 1)
		throw std::runtime_error("Multiple meshes are not supported");
	if (scene->mNumMeshes == 0)
		throw std::runtime_error("No meshes available");
	aiMesh *mesh = scene->mMeshes[0];

	std::cout << "Mesh " << (mesh->HasTextureCoords(0) ? "" : "Doesn't ")
			  << "have texture coordinates" << std::endl;
	std::cout << "Mesh " << (mesh->HasNormals() ? "" : "Doesn't ")
			  << "have normals" << std::endl;
	std::cout << "Mesh " << (mesh->HasPositions() ? "" : "Doesn't ")
			  << "have positions coordinates" << std::endl;
	std::cout << "Mesh " << (mesh->HasFaces() ? "" : "Doesn't ") << "have faces"
			  << std::endl;
	std::cout << "Mesh " << (mesh->HasVertexColors(0) ? "" : "Doesn't ")
			  << "have vertex colors" << std::endl;

	std::vector<float> vertexData;
	vertexData.reserve(mesh->mNumVertices * (3 + 3 * (mesh->HasNormals()) +
											 2 * (mesh->HasTextureCoords(0))));

	aiVector3D *vertices = mesh->mVertices;
	vertexData.insert(vertexData.end(), reinterpret_cast<float *>(vertices),
					  reinterpret_cast<float *>(&vertices[mesh->mNumVertices]) +
						  2);
	std::cout << "Vertices:" << std::endl;
	printData(vertexData.begin(), vertexData.end(), 3);

	if (mesh->HasNormals())
	{
		aiVector3D *normals = mesh->mNormals;
		int currentSize		= vertexData.size();
		vertexData.insert(
			vertexData.end(), reinterpret_cast<float *>(normals),
			reinterpret_cast<float *>(&normals[mesh->mNumVertices]) + 2);
		std::cout << "Normals:" << std::endl;
		printData(vertexData.begin() + currentSize, vertexData.end(), 3);
	}

	if (mesh->HasTextureCoords(0))
	{
		aiVector3D *textures = mesh->mTextureCoords[0];
		int currentSize		 = vertexData.size();
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			vertexData.push_back(textures[i].x);
			vertexData.push_back(textures[i].y);
		}
		std::cout << "Texture Coordinates:" << std::endl;
		printData(vertexData.begin() + currentSize, vertexData.end(), 2);
	}

	std::vector<unsigned int> indexData;
	std::cout << "Number faces: " << mesh->mNumFaces << std::endl;
	indexData.reserve(mesh->mNumFaces * 3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		auto face = mesh->mFaces[i];
		if (face.mNumIndices != 3)
			throw std::runtime_error("Nontriangular faces are not supported");
		indexData.insert(indexData.end(), face.mIndices,
						 face.mIndices + face.mNumIndices);
	}
	std::cout << "Faces:" << std::endl;
	printData(indexData.begin(), indexData.end(), 3);

	return 0;
}
