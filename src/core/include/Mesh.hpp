#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

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

class Mesh {
public:
	vector<Vertex_s> vertices;
	vector<unsigned int> indeces;
    vector<pair<TexturePath, MapType>> textures;
    Mesh(const vector<Vertex_s>& vertices, 
         const vector<unsigned int>& indices, 
         const vector<pair<TexturePath, MapType>>& textures);
    void Draw(Shader shader);

private:
    Mesh();
    /*  Render data  */
    ElementArrayBuffer vertexBuffer;
    /*  Functions    */
    void setupMesh();
};
