#include <FrameBuffer.hpp>
#include <types.hpp>

using namespace std;
void FrameBuffer::create()
{
    glGenFramebuffers(1, &m_FrameBuffer);
}

void FrameBuffer::bind()
{;
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
}

void FrameBuffer::bindDefault()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::createColorTexture(int height, int width)
{
    if(m_ColorTexture)
    {
        pr_warn("color texture already exists for this framebuffer");
        return;
    }
    m_ColorTexture = move(unique_ptr<Texture>(new Texture));
    m_ColorTexture->createColorTexture(height, width);
    m_ColorTexture->activate(0) ;
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture->getId(), 0);
    bindDefault();
}

void FrameBuffer::createDepthTexture(int height, int width)
{
    if(m_DepthTexture)
    {
        pr_warn("depth texture already exists for this framebuffer");
        return;
    }
    m_DepthTexture = move(unique_ptr<Texture>(new Texture));
    m_DepthTexture->createDepthTexture(height, width);
    m_DepthTexture->activate(0);
    bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture->getId(), 0);
    bindDefault();
}

void FrameBuffer::createStencilTexture(){}

void FrameBuffer::bindColorTexture(GLenum textureUnit)
{
    m_ColorTexture->activate(textureUnit);
}

void FrameBuffer::bindDepthTexture(GLenum textureUnit)
{
    m_DepthTexture->activate(textureUnit);
}

