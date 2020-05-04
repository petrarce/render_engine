#pragma once
#include <string>
#include <GL/glew.h>
enum class ColorChanel {
   RGB, RGBA
};

class Texture {
public:
    Texture();
    ~Texture();

    void activate(int textureUnit);
    unsigned int load2DTexture(std::string path, bool gamma = true);
    void createDepthTexture(int width, int height);
    void createColorTexture(int width, int height);
    void free();
    GLuint getId(){
        return textureId;
    }

private:
    GLuint textureId;
};
