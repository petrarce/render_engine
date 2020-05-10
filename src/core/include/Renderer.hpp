#pragma once
#include <Buffer.hpp>
class Renderer
{
public:
    static void draw(Buffer* buffer, GLenum primitiveType)
    {
        if(!buffer)
            return;

        buffer->bind();
        unsigned int bufferSize = buffer->size();
        switch(buffer->type())
        {
        case Buffer::ArrayBufferType:
            glDrawArrays(primitiveType, 0, bufferSize); 
            break;
        case Buffer::ElementArrayBufferType:
            glDrawElements(primitiveType, bufferSize, GL_UNSIGNED_INT, 0); 
            break;
        default:
            pr_warn("unknown buffer type...");
            assert(0);
        }
        Buffer::bindDefault();
    }

    Renderer() = delete;
    ~Renderer() = delete;
};
