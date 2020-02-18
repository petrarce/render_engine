#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <Mesh.hpp>
#include <string>
#include <vector>

using namespace std;

class Model 
{
    public:
        /*  Functions   */
        Model(){};
        Model(const char *path)
        {
            loadModel(path);
        }
        void Draw(Shader shader);	
        const Model& operator=(const string& path){
            loadModel(path);
            return *this;
        };
        void loadNormalMapTexture(string path);

    private:
        /*  Model Data  */
        vector<Mesh> meshes;
        string directory;
        /*  Functions   */
        void loadModel(string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        vector<Texture_s> loadMaterialTextures(aiMaterial *mat, aiTextureType type, 
                                             string typeName);
};