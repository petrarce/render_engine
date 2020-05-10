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
    void Draw(Shader shader);
    const Model& operator=(const string& path){
        loadModel(path);
        return *this;
    }
    void loadNormalMapTexture(string path);
    const Mesh& getMesh(int meshId);
    const vector<MeshID>& getMeshList() const;


private:
    /*  Model Data  */
    vector<MeshID> meshes;
    string directory;
    /*  Functions   */
    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    MeshID processMesh(aiMesh *mesh, const aiScene *scene);
    vector<pair<TexturePath, MapType>> loadMaterialTextures(
            aiMaterial *mat, 
            aiTextureType type, 
            MapType mapType);
};
