#include <types.hpp>

#include <string>
#include <TextureManager.hpp>
#include <Texture.hpp>
#include <QFile>
#include <QString>

using namespace std;

void TextureManager::createTexture(string path, 
                                          unsigned int type)
{
    TextureManager& texManager = getInstance();
    if(texManager.textures.find(path) != texManager.textures.end()){
        return;
    }
    
    QFile file(path.c_str());
    if (!file.exists()) {
        pr_err("failed to find texture file: %s", path.c_str());
        return;
    }
    unique_ptr<Texture> newTex;
    switch(type){

    case GL_TEXTURE_1D:
        pr_err("texture 1D is not supported");
        break;
    

    case GL_TEXTURE_3D:
        pr_err("texture 3D are not supported");
        break;
    default:
        pr_warn("Unknown texture type. Generating texture 2D");
    case GL_TEXTURE_2D:
        newTex = unique_ptr<Texture>(new Texture());
        if(newTex->load2DTexture(path, true) == -1){
            pr_err("failed to load 2D texture from %s", path.c_str());
            return;
        }
        texManager.textures[path] = move(newTex);
        break;
    }
}

void TextureManager::deleteTexture(string path)
{
    TextureManager& texManager = getInstance();
    if(texManager.textures.find(path) != texManager.textures.end()){
        texManager.textures[path]->free();
    }
}

void TextureManager::activateTexture(string path, unsigned int textureUnit)
{
    TextureManager& texManager = getInstance();
    if(texManager.textures.find(path) != texManager.textures.end()){
        assert(texManager.textures[path] != nullptr);
        texManager.textures[path]->activate(textureUnit);
    }
}


//void TextureManager::createTexture(std::string path, 
//                          unsigned int type)
//{
//    TextureManager& manager = TextureManager::getInstance();
//    manager.createTextureL(path, type);
//}

//void TextureManager::deleteTexture(std::string path)
//{
//    TextureManager& manager = TextureManager::getInstance();
//    manager.deleteTextureL(path);
//}
//void TextureManager::activateTexture(std::string path,
//                    unsigned int unit)
//{
//    TextureManager& manager = TextureManager::getInstance();
//    manager.activateTextureL(path, unit);
//}

