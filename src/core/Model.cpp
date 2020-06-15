#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <Model.hpp>
#include <types.hpp>
#include <TextureManager.hpp>
#include <MeshManager.hpp>


void Model::loadModel(string path)
{
    pr_info("loading model from %s", path.c_str());
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);    
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    this->directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
    pr_info("model from %s loaded sucessfully", path.c_str());
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    pr_info("processing model node");
    // process all the node's meshes (if any)

    pr_info("processing meshes");
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        meshes.push_back(processMesh(mesh, scene));         
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

MeshID Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex_s> vertices;
    vector<unsigned int> indices;
    vector<pair<TexturePath, MapType>> textures;

    pr_info("Loading mesh %s", mesh->mName.C_Str());
    if(!mesh->HasPositions()){
        throw invalid_argument("ERROR::Model: Vertex_s positions are not present in the loaded asset");
    }
    if(!mesh->HasNormals()){
        throw invalid_argument("ERROR::Model: Vertex_s normals are not present in the loaded asset");
    }
    if(!mesh->HasFaces()){
        throw invalid_argument("ERROR::Model: faces are not present in the loaded asset");
    }


    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex_s vertex;
        // process vertex positions
        glm::vec3 vector; 
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z; 
        vertex.Position = vector;

        //process vertex normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;  

        if(mesh->mTextureCoords[0]) // does the mesh contain Texture_s coordinates?
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else{
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);  
        }


        vertices.push_back(vertex);
    }

    //process face indexes
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }  

    // process material textures
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        vector<pair<TexturePath, MapType>> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, diffuse_map);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<pair<TexturePath, MapType>> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, specular_map);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());    
        vector<pair<TexturePath, MapType>> normalMaps = loadMaterialTextures(material, 
                                            aiTextureType_NORMALS, normal_map);
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());    
    }

    return MeshManager::createItem(vertices, indices, textures);
}

vector<pair<TexturePath, MapType>> Model::loadMaterialTextures(
        aiMaterial *mat, 
        aiTextureType type, 
        MapType mapType)
{
    vector<pair<TexturePath, MapType>> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        string pathToTexture = /*directory + "/" +*/ string(str.C_Str());
        TextureManager::createItem(pathToTexture, GL_TEXTURE_2D);
        textures.push_back(make_pair(pathToTexture, mapType));
    }
    return textures;
}

const vector<MeshID>& Model::getMeshList() const{
    return this->meshes;
}
