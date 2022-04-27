#pragma once
#include <iostream>

class Buffer {
protected:
    GLuint ABO;
    
public:
    enum BufferType {
        ArrayBufferType = 0,
        ElementArrayBufferType
    };
    
    Buffer(): ABO(0){}
    virtual ~Buffer(){}
    void bind() const
    {
        assert(ABO != 0);
        glBindVertexArray(ABO);
        assert(glGetError() == GL_NO_ERROR);
    }

    void defineAttribute(GLint index, 
                         GLsizei stride,
                         unsigned int numComponents = 3,
                         bool normalize = false,
                         GLenum type = GL_FLOAT,
                         const void* offsetToFirstElement = 0)
    {
        bind();
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 
                              numComponents, 
                              type, 
                              normalize, 
                              stride, 
                              offsetToFirstElement);
        assert(glGetError() == GL_NO_ERROR);
    }
    
    void create()
    {
        assert(ABO == 0);
        glGenVertexArrays(1, &ABO);
        assert(glGetError() == GL_NO_ERROR);
    }
    
    virtual BufferType type() const = 0;

    static void bindDefault()
    {
        glBindVertexArray(0);
    }

    virtual unsigned int size() const = 0;
    
    const Buffer& operator=(const Buffer&) = delete;
    Buffer(const Buffer&) = delete;
};

class ArrayBuffer : public Buffer
{
protected:
    GLuint VBO;
    unsigned int mNumData;
public:
    ArrayBuffer(): 
          VBO(0)
        , mNumData(0){}
    virtual ~ArrayBuffer()
    {
        std::cerr << "~ArrayBuffer\n";
        glDeleteBuffers(1, &VBO);
        assert(glGetError() == GL_NO_ERROR);
    }
    
    void create(const void* verticesData, 
                unsigned int verticesSize,
                unsigned int numVertices,
                GLenum usage = GL_STATIC_DRAW)
    {
        assert(VBO == 0);
        Buffer::create();
        Buffer::bind();
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verticesSize * numVertices, verticesData, usage);
        assert(glGetError() == GL_NO_ERROR);
        mNumData = numVertices;
    }
    virtual BufferType type() const override {
        return ArrayBufferType;
    }
    
    unsigned int numData() const
    {
        return mNumData;
    }
    
    virtual unsigned int size() const override
    {
        return numData();
    }
};

class ElementArrayBuffer : public ArrayBuffer
{
private:
    GLuint EBO;
    unsigned int mNumIndecis;
public:
    ElementArrayBuffer():
          EBO(0)
        , mNumIndecis(0){}
    virtual ~ElementArrayBuffer()
    {
        std::cerr << "~ElementArrayBuffer\n";
        glDeleteBuffers(1, &EBO);
    }
    unsigned int numIndeces() const
    {
        return mNumIndecis;
    }
    
    void create(const void* verticesData, 
                unsigned int verticeSize, 
                unsigned int numVertices,
                const unsigned int* indexData, 
                GLsizei indecisNum,
                GLenum usage = GL_STATIC_DRAW)
    {
        assert(EBO == 0);
        ArrayBuffer::create(verticesData, verticeSize, numVertices, usage);
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                     indecisNum * sizeof(*indexData), 
                     indexData, 
                     usage);
        assert(glGetError() == GL_NO_ERROR);
        mNumIndecis = indecisNum;
    }
    
    virtual unsigned int size() const override
    {
        return numIndeces();
    }
    
    virtual BufferType type() const override
    {
        return ElementArrayBufferType;
    }
};
