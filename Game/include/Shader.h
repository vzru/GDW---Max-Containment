#pragma once

#include <string>

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

	// clears all data from OpenGL
	Shader* unload();

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
	Shader* addAttribute(unsigned int index, const std::string &attribName);

	// send uniform data to the shaders
	Shader* sendUniform(const std::string &name, int integer);
	Shader* sendUniform(const std::string &name, unsigned int unsignedInteger);
	Shader* sendUniform(const std::string &name, float scalar);
	Shader* sendUniform(const std::string &name, const glm::vec2 &vector);
	Shader* sendUniform(const std::string &name, const glm::vec3 &vector);
	Shader* sendUniform(const std::string &name, const glm::vec4 &vector);
	Shader* sendUniformMat3(const std::string &name, float *matrix, bool transpose);
	Shader* sendUniformMat4(const std::string &name, float *matrix, bool transpose);

	bool isLoaded() const;
private:
	bool loaded = false;

	GLuint vertexShader = 0;
	GLuint fragShader = 0;
	GLuint program = 0;

	std::string readFile(const std::string &fileName) const;

	bool compileShader(GLuint shader) const;

	Shader* outputShaderLog(GLuint shader);

	Shader* outputProgramLog();
};