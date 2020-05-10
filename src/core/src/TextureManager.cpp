#include <types.hpp>

#include <string>
#include <TextureManager.hpp>
#include <Texture.hpp>
#include <QFile>
#include <QString>

using namespace std;

void TextureManager::createItem(string path, 
                                          GLint type)
{
    auto& texManager = getInstance();
    if(texManager.mItemList.find(path) != texManager.mItemList.end()){
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
        texManager.mItemList[path] = move(newTex);
        break;
    }
}

void TextureManager::createItem(std::string path) 
{
    createItem(path, GL_TEXTURE_2D);
}


void TextureManager::deleteItem(string path) 
{
    auto& texManager = getInstance();
    if(texManager.mItemList.find(path) != texManager.mItemList.end()){
        texManager.mItemList.erase(path);
    }
}

void TextureManager::activateItem(string path, GLint textureUnit)
{
    auto& texManager = getInstance();
    if(texManager.mItemList.find(path) != texManager.mItemList.end()){
        assert(texManager.mItemList[path] != nullptr);
        texManager.mItemList[path]->activate(textureUnit);
    }
}
void TextureManager::activateItem(std::string key) 
{
    activateItem(key, 0);
}


