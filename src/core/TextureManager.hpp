#pragma once
#include <string>
#include <Texture.hpp>
#include <unordered_map>
#include <memory>
#include <Manager.hpp>
#include <Singleton.hpp>

typedef unsigned int TextureId;
class TextureManager : public Manager<std::string,
                                      Texture,
                                      TextureManager>
{
    
public:
    static void createItem(std::string path);
    static void createItem(string path, GLint type);
    
    static void deleteItem(std::string key);
    static void activateItem(std::string key);
    static void activateItem(std::string key, 
                                GLint textureUnit);

    static void setItemParameters(unsigned int paramType, 
                                    unsigned int value);
};
