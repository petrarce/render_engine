#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <Model.hpp>
#include <types.hpp>

void Model::Draw(Shader shader)
{
    for(auto& mesh : meshes){
        mesh.Draw(shader);
    }
}

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = true)
{
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID = -1;

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        glGenTextures(1, &textureID);
        GLenum internalFormat;
        GLenum dataFormat;

        if (nrComponents == 1){
            internalFormat = dataFormat = GL_RED;
        }
        else if (nrComponents == 3){
            dataFormat = GL_RGB;
            internalFormat = (gamma)?GL_SRGB:dataFormat;
        }
        else if (nrComponents == 4){
            dataFormat = GL_RGBA;
            internalFormat = (gamma)?GL_SRGB_ALPHA:dataFormat;

        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture_s failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

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

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Vertex_s> vertices;
    vector<unsigned int> indices;
    vector<Texture_s> textures;

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
        vector<Texture_s> diffuseMaps = loadMaterialTextures(material, 
                                            aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture_s> specularMaps = loadMaterialTextures(material, 
                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());    
        vector<Texture_s> normalMaps = loadMaterialTextures(material, 
                                            aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());    
    }

    return Mesh(vertices, indices, textures);}

vector<Texture_s> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                     string typeName)
{
    vector<Texture_s> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        Texture_s Texture;
        Texture.id = TextureFromFile(str.C_Str(), directory);
        Texture.type = typeName;
        Texture.path = string(str.C_Str());
        textures.push_back(Texture);
    }
    return textures;
}

void Model::loadNormalMapTexture(string path){
    Texture_s Texture;
    pr_info("Loading normal map %s ", path.c_str());
    string directory = path.substr(0, path.find_last_of('/'));
    Texture.id = TextureFromFile(path.c_str(), directory, false);
    if(Texture.id == -1){
        return;
    }
    Texture.type = "texture_normal";
    Texture.path = path.c_str();
    for(Mesh& mesh : meshes){
        mesh.textures.push_back(Texture);
    }
    pr_info("Normal map %s loaded sucessfully", (directory + path).c_str());
}