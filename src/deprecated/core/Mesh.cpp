#include <Mesh.hpp>
#include <types.hpp>
#include <TextureManager.hpp>
#include <Renderer.hpp>

Mesh::Mesh(const vector<Vertex_s>& vertices, 
			const vector<unsigned int>& indeces, 
			const vector<pair<TexturePath, MapType>>& textures)
{
	this->vertices = vertices;
	this->indeces = indeces;
	this->textures = textures;
	vertexBuffer.create(vertices.data(), sizeof(Vertex_s), vertices.size(),
						indeces.data(), indeces.size(), GL_STATIC_DRAW);
	vertexBuffer.defineAttribute(0, sizeof(Vertex_s), 3, false, GL_FLOAT, (void*)0);
	vertexBuffer.defineAttribute(1, sizeof(Vertex_s), 3, false, GL_FLOAT, (void*)offsetof(Vertex_s, Normal));
	vertexBuffer.defineAttribute(2, sizeof(Vertex_s), 2, false, GL_FLOAT, (void*)offsetof(Vertex_s, TexCoords));
}
void Mesh::setup(Shader& shader) const
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    shader["useDiffuse"] = 0;
    shader["useSpecular"] = 0;
    shader["useNormal"] = 0;
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
            shader["useDiffuse"] = 1;
            break;
        case specular_map:
            name = "texture_specular";
            number = std::to_string(specularNr++);
            shader["useSpecular"] = 1;
            break;
        case normal_map:
            name = "texture_normal";
            number = std::to_string(normalNr++);
            shader["useNormal"] = 1;
            break;
        default:
            pr_warn("undefinde map type for the texture. Treading it as diffuse map");
        }

        shader[name + number] = int(i);
        TextureManager::activateItem(textures[i].first, i);
    }
}
