#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>

#include <Shader.hpp>

using namespace std;

typedef struct {
    glm::vec3  Position;
    glm::vec3  Normal;
    glm::vec2  TexCoords;
} Vertex_s;

typedef struct {
    unsigned int id;
    string type;
    string path;
} Texture_s;

class Mesh {
public:
	vector<Vertex_s> vertices;
	vector<unsigned int> indeces;
    vector<Texture_s> textures;
    Mesh(const vector<Vertex_s>& vertices, const vector<unsigned int>& indices, const vector<Texture_s>& textures);
    void Draw(Shader shader);
private:
    /*  Render data  */
    unsigned int VAO, VBO, EBO;
    /*  Functions    */
    void setupMesh();
};