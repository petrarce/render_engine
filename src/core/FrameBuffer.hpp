#pragma once
#include <GL/glew.h>
#include <Texture.hpp>
#include <memory>

class FrameBuffer {

public:
    void create();
    
    void bind();
    static void bindDefault();
    
    void createColorTexture(int height, int width);
    void createDepthTexture(int height, int width);
    void createStencilTexture();
    
    void bindColorTexture(GLenum);
    void bindDepthTexture(GLenum);
    
    FrameBuffer() = default;
private:
    GLuint m_FrameBuffer;
    std::unique_ptr<Texture> m_ColorTexture;
    std::unique_ptr<Texture> m_DepthTexture;
};
