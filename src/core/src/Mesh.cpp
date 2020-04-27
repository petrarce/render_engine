#include <Mesh.hpp>
#include <types.hpp>
#include <TextureManager.hpp>

Mesh::Mesh(const vector<Vertex_s>& vertices, 
			const vector<unsigned int>& indeces, 
			const vector<pair<TexturePath, MapType>>& textures)
{
	this->vertices = vertices;
	this->indeces = indeces;
	this->textures = textures;

	setupMesh();
}
void Mesh::Draw(Shader shader)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    shader["useDiffuse"] = 0;
    shader["useSpecular"] = 0;
    shader["hasNormalMap"] = 0;
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        string number;
        MapType mapType =  textures[i].second;
        string name;
        switch(mapType)
        {
        case diffuse_map:
            name = "texture_diffuse";
            number = std::to_string(diffuseNr++);
            shader["hasDiffuseMap"] = 1;
            break;
        case specular_map:
            name = "texture_specular";
            number = std::to_string(specularNr++);
            shader["hasSpecularMap"] = 1;
            break;
        case normal_map:
            name = "texture_normal";
            number = std::to_string(normalNr++);
            shader["hasNormalMap"] = 1;
            break;
        default:
            pr_warn("undefinde map type for the texture. Treading it as diffuse map");
        }

        shader[name + number] = int(i);
        TextureManager::activateTexture(textures[i].first, i);
    }

    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, this->indeces.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex_s), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indeces.size() * sizeof(unsigned int), 
                 &indeces[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_s), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex_s), (void*)offsetof(Vertex_s, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex_s), (void*)offsetof(Vertex_s, TexCoords));

    glBindVertexArray(0);
}

