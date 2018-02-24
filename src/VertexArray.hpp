#pragma once
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexPosition;
    glm::vec3 Tangent;
};

struct VertexT{
    glm::vec3 Position;
    glm::vec2 TexPosition;
    glm::vec3 Normal;
};

struct VertexN{
    glm::vec3 Position;
    glm::vec3 Normal;
};

struct VertexS{
    glm::vec3  Position;
    glm::vec3  Normal;
    glm::vec4  Weight;
    glm::ivec4 BoneId;
};

enum class GlTypes { Float, Int };
enum class GlBool { True, False };
class VertexArray{
public:
    VertexArray(){}
    ~VertexArray(){}
    void
    createVertexArray(const std::vector<float> &vertices );
    void
    createVertexArray(const std::vector<glm::vec3> &vertices );
    void
    createVertexArray(const std::vector<Vertex> &vertices );
    void
    createIndexBuffer(const std::vector<float> &vertices, const std::vector<unsigned int> &indices );
    void
    createIndexBuffer(const std::vector<VertexT> &vertices, const std::vector<unsigned int> &indices );
    void
    createIndexBuffer(const std::vector<VertexN> &vertices, const std::vector<unsigned int> &indices );
    void
    createIndexBuffer(const std::vector<VertexS> &vertices, const std::vector<unsigned int> &indices );
    void
    createVertexArray( const std::vector<VertexN> &vertices );
    void
    bindVBO(std::vector<glm::ivec4> const & vertices);

    void
    bindVBO(std::vector<glm::vec4> const & vertices);

    GLuint
    getVertexArray();
    int
    getSizeOfArray(){ return m_NoV; }
    void
    describeVertexArray( int shaderPos, int number, GlTypes t, int nov, GlBool normalized, int stride );

    void
    bindVertexArray();

private:
    GLuint m_VBO;
    GLuint m_VAO;
    GLuint m_IBO;
    int m_NoV;
};