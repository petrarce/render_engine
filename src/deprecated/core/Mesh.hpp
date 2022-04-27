#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <Renderable.hpp>

#include <Shader.hpp>
#include <Buffer.hpp>

using namespace std;

typedef struct {
    glm::vec3  Position;
    glm::vec3  Normal;
    glm::vec2  TexCoords;
} Vertex_s;

enum MapType {
    unknown_map = 0,
    normal_map,
    diffuse_map,
    specular_map
};
typedef string TexturePath;

class Mesh : Renderable
{
public:
	vector<Vertex_s> vertices;
	vector<unsigned int> indeces;
    vector<pair<TexturePath, MapType>> textures;
    Mesh(const vector<Vertex_s>& vertices, 
         const vector<unsigned int>& indices, 
         const vector<pair<TexturePath, MapType>>& textures);
    void setup(Shader& shader) const override;
    const Buffer* buffer() const {return &vertexBuffer;}

private:
    Mesh();
    /*  Render data  */
    ElementArrayBuffer vertexBuffer;
};
