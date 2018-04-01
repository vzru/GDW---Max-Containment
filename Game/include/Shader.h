#pragma once

#include <string>
#include <unordered_map>

#include <GL\glew.h>
#include <glm\glm.hpp>

class Shader
{
public:
	Shader();
	~Shader();

	// Loads a vertex shader and a fragment shader
	// and places them in a program
	bool load(const std::string &vertFile, const std::string &fragFile);
	bool load(const std::string &vertFile, const std::string &geoFile, const std::string &fragFile);
	GLuint load(const std::string &file, GLenum type);

	// clears all data from OpenGL
	void unload();
	void unload(GLuint& handle);

	bool linkProgram();

	// use the shader
	Shader* bind();

	// detach the shader from use
	Shader* unbind();

	// Returns -1 if attribute does not exist
	int getAttribLocation(const std::string &attribName);

	// Returns -1 if uniform doesn't exist
	int getUniformLocation(const std::string &uniformName);

	// Requires a re-link before OpenGL will register the change
	void addAttribute(unsigned int index, const std::string &attribName);

	// send uniform data to the shaders
	Shader* sendUniform(const std::string &name, int integer);
	Shader* sendUniform(const std::string &name, unsigned int uInteger);
	Shader* sendUniform(const std::string &name, float scalar);
	Shader* sendUniform(const std::string &name, const glm::vec2 &vector);
	Shader* sendUniform(const std::string &name, const glm::vec3 &vector);
	Shader* sendUniform(const std::string &name, const glm::vec4 &vector);
	Shader* sendUniformMat3(const std::string &name, float *matrix, bool transpose);
	Shader* sendUniformMat4(const std::string &name, float *matrix, bool transpose);
	void sendUniform(const std::string &name, const glm::mat2 &matrix, bool transpose = false);
	void sendUniform(const std::string &name, const glm::mat3 &matrix, bool transpose = false);
	void sendUniform(const std::string &name, const glm::mat4 &matrix, bool transpose = false);
	template<class T>
	void sendUniform(const std::string &name, std::vector<T> vector);

	bool isLoaded() const;
	bool loaded = false;
private:

	GLuint vertShader = 0;
	GLuint geoShader = 0;
	GLuint fragShader = 0;
	GLuint program = 0;

	std::unordered_map<std::string, GLint> uniformLocations;

	std::string readFile(const std::string &fileName) const;

	void outputShaderLog(GLuint shader) const;
	void outputProgramLog() const;
};