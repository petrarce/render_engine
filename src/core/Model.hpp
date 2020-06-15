#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Mesh.hpp>
#include <string>
#include <vector>
#include <Texture.hpp>
#include <MeshManager.hpp>

using namespace std;

class Model 
{
public:
    /*  Functions   */
    Model(){}
    Model(const char *path)
    {
        loadModel(path);
    }
    const Model& operator=(const string& path){
        loadModel(path);
        return *this;
    }
    void loadNormalMapTexture(string path);
    const vector<MeshID>& getMeshList() const;
    void loadModel(string path);

private:
    /*  Model Data  */
    vector<MeshID> meshes;
    string directory;
    /*  Functions   */
    void processNode(aiNode *node, const aiScene *scene);
    MeshID processMesh(aiMesh *mesh, const aiScene *scene);
    vector<pair<TexturePath, MapType>> loadMaterialTextures(
            aiMaterial *mat, 
            aiTextureType type, 
            MapType mapType);
};
