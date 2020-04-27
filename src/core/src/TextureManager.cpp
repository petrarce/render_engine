#include <types.hpp>

#include <string>
#include <TextureManager.hpp>
#include <Texture.hpp>
#include <QFile>
#include <QString>

using namespace std;

void TextureManager::createTextureL(string path, 
                                          unsigned int type)
{
    if(textures.find(path) != textures.end()){
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
        textures[path] = move(newTex);
        break;
    }
}

void TextureManager::deleteTextureL(string path)
{
    if(textures.find(path) != textures.end()){
        textures[path]->free();
    }
}

void TextureManager::activateTextureL(string path, unsigned int textureUnit)
{
    if(textures.find(path) != textures.end()){
        assert(textures[path] != nullptr);
        textures[path]->activate(textureUnit);
    }
}


void TextureManager::createTexture(std::string path, 
                          unsigned int type)
{
    TextureManager& manager = TextureManager::getInstance();
    manager.createTextureL(path, type);
}

void TextureManager::deleteTexture(std::string path)
{
    TextureManager& manager = TextureManager::getInstance();
    manager.deleteTextureL(path);
}
void TextureManager::activateTexture(std::string path,
                    unsigned int unit)
{
    TextureManager& manager = TextureManager::getInstance();
    manager.activateTextureL(path, unit);
}

