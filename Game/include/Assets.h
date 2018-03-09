
#include <unordered_map>

#define ASSET_PATH std::string("assets/")
#define TEXTURE_PATH(file)	(ASSET_PATH + std::string("textures/") + (file))
#define SHADER_PATH(file)	(ASSET_PATH + std::string("shaders/") + (file))
#define SOUND_PATH(file)	(ASSET_PATH + std::string("sounds/") + (file))
#define MESH_PATH(file)		(ASSET_PATH + std::string("meshes/") + (file))

class Mesh;		class Material;
class Shader;	class Texture;
class ShaderProgram;

class Assets {
	Assets() {}
	static Assets *assets;
public:
	static Assets *getInstance();
	~Assets();

	// loaders
	Mesh *loadMesh(const std::string &name, const std::string &file);
	Texture *loadTexture(const std::string &name, const std::string &file);

	// assets
	std::unordered_map<std::string, Mesh*> meshes;
	std::unordered_map<std::string, Texture*> textures;
};