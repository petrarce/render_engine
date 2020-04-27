#pragma once
#include <string>
#include <Texture.hpp>
#include <unordered_map>
#include <memory>

typedef unsigned int TextureId;
class TextureManager {
private:
    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;

public:
    static void createTexture(std::string path, 
                              unsigned int type);
    static void deleteTexture(std::string);
    static void activateTexture(std::string,
                                unsigned int unit = 0);
    static void setTextureParameter(unsigned int paramType, 
                                    unsigned int value);
    
    void createTextureL(std::string path, 
                              unsigned int type);
    void deleteTextureL(std::string);
    void activateTextureL(std::string,
                                unsigned int unit);
    static TextureManager& getInstance() {
        static TextureManager inst;
        return inst;
    }
private:

    TextureManager(){}
    TextureManager(const TextureManager&) = delete;
    const TextureManager& operator=(const TextureManager&) = delete;
    ~TextureManager(){}
};
