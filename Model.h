#include "Mesh.h"
#include <SOIL.h>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

GLint TextureFromFile(const char* path, string directory);
class Model
{
public:

	Model(GLchar* path);
	void Draw(Shader shader);
private:

	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,string typeName);
	
};