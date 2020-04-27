#include <Texture.hpp>
#include <stb_image.h>
#include <QFile>
#include <types.hpp>

using namespace std;
int
mapToGl(int textureUnit) {
    const int lut[] = {
        GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5
    };
    return lut[textureUnit];
}


unsigned int
Texture::load2DTexture(string path, bool gamma) 
{
    
    if(textureId){
        free();
        textureId = 0;
    }

    QFile file(path.c_str());
    if(!file.exists()){
        pr_err("cannot load texture: %s  doesnt exists", path.c_str());
        return -1;
    }
    
    int width, height, nrComponents;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        glGenTextures(1, &textureId);
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

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        
        return 0;
    }
    pr_err("cannot load texture data from %s", path.c_str());
    return -1;
}

void
Texture::activate(int textureUnit) {
    if(textureId == 0){
        return;
    }
    glActiveTexture(mapToGl( textureUnit));
    glBindTexture(GL_TEXTURE_2D, textureId);
}

void
Texture::createDepthTexture(int width, int height) {
    if(textureId){
        free();
    }
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

GLuint
Texture::getTexture() {
    return textureId;
}


void Texture::free()
{
    glDeleteTextures(1, &textureId);
    textureId = 0;
}
