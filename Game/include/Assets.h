
#include <unordered_map>

#define ASSET_PATH std::string("assets/")
#define TEXTURE_PATH(file)	(ASSET_PATH + std::string("textures/") + (file))
#define SHADER_PATH(file)	(ASSET_PATH + std::string("shaders/") + (file))
#define SOUND_PATH(file)	(ASSET_PATH + std::string("sounds/") + (file))
#define MESH_PATH(file)		(ASSET_PATH + std::string("meshes/") + (file))

class Mesh;		class Material;
class Shader;	class Texture;
class Sound;
class AnimationMesh;

class Assets {
	Assets() {}
	static Assets *assets;
public:
	static Assets *getInstance();
	~Assets();

	// loaders
	Mesh* loadMesh(const std::string &name, const std::string &file);
	AnimationMesh* loadAnimationMesh(const std::string &name, const std::string &file1, const std::string &file2);
	void loadAnimation(const std::string &name, const std::string &file, const std::string &extension, const int num);
	Texture* loadTexture(const std::string &name, const std::string &file);
	Sound* loadSound(const std::string &name, const std::string &file, const bool loop = false, const int dim = 3);
	Shader* loadShader(const std::string &name, const std::string &vertFile, const std::string &fragFile);
	Shader* loadShader(const std::string &name, const std::string &vertFile, const std::string &geoFile, const std::string &fragFile);

	// assets
	std::unordered_map<std::string, Mesh*> meshes;
	std::unordered_map<std::string, AnimationMesh*> aMeshes;
	std::unordered_map<std::string, Texture*> textures;
	std::unordered_map<std::string, Sound*> sounds;
	std::unordered_map<std::string, Shader*> shaders;
};