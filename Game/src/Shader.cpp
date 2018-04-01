#include <fstream>
#include <iostream>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Shader::Shader() {}

Shader::~Shader() {
	if (loaded)	unload();
}

bool Shader::load(const std::string& vertFile, const std::string& fragFile) {
	vertShader = load(vertFile, GL_VERTEX_SHADER);
	fragShader = load(fragFile, GL_FRAGMENT_SHADER);

	// Setup program object
	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);

	if (!linkProgram()) {
		std::cout << "Shader program failed to link." << std::endl;
		outputProgramLog();
		unload();
		return false;
	}

	return loaded = true;
}

bool Shader::load(const std::string& vertFile, const std::string& geoFile, const std::string& fragFile) {
	vertShader = load(vertFile, GL_VERTEX_SHADER);
	geoShader = load(geoFile, GL_GEOMETRY_SHADER);
	fragShader = load(fragFile, GL_FRAGMENT_SHADER);

	// Setup program object
	program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, geoShader);
	glAttachShader(program, fragShader);

	if (!linkProgram()) {
		std::cout << "Shader program failed to link." << std::endl;
		outputProgramLog();
		unload();
		return false;
	}

	return loaded = true;
}

GLuint Shader::load(const std::string& file, GLenum type) {
	GLuint handle = glCreateShader(type);
	std::string source = readFile(file);
	const GLchar *temp = static_cast<const GLchar*>(source.c_str());
	//std::cout << temp << std::endl;
	glShaderSource(handle, 1, &temp, NULL);
	
	std::cout << "Loading shader: " << file << std::endl;

	glCompileShader(handle);
	GLint success;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

	if (!success) {
		std::cout << "Shader failed to compile." << std::endl;
		outputShaderLog(handle);
		unload();
		system("pause");
	}

	return handle;
}

void Shader::unload() {
	unload(vertShader);
	unload(geoShader);
	unload(fragShader);
	if (program != 0) {
		glDeleteProgram(program);
		program = 0;
	}

	loaded = false;
}

void Shader::unload(GLuint& handle) {
	if (handle != 0) {
		glDetachShader(program, handle);
		glDeleteShader(handle);
		handle = 0;
	}
}

bool Shader::linkProgram() {
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	return success == GL_TRUE;
}

Shader* Shader::bind() {
	glUseProgram(program);
	return this;
}

Shader* Shader::unbind() {
	glUseProgram(GL_NONE);
	return this;
}

void Shader::addAttribute(unsigned int index, const std::string& attribName) {
	glBindAttribLocation(program, index, attribName.c_str());
}

int Shader::getAttribLocation(const std::string& attribName) {
	return glGetAttribLocation(program, attribName.c_str());
}

int Shader::getUniformLocation(const std::string& uniformName) {
	if (uniformLocations.find(uniformName) == uniformLocations.end())
		uniformLocations.emplace(uniformName, glGetUniformLocation(program, uniformName.c_str()));
	return uniformLocations.at(uniformName);
}

Shader* Shader::sendUniform(const std::string & name, int integer) {
	GLint location = getUniformLocation(name);
	glUniform1i(location, integer);
	return this;
}

Shader* Shader::sendUniform(const std::string & name, unsigned int uInteger) {
	GLint location = getUniformLocation(name);
	glUniform1ui(location, uInteger);
	return this;
}

Shader* Shader::sendUniform(const std::string & name, float scalar) {
	GLint location = getUniformLocation(name);
	glUniform1f(location, scalar);
	return this;
}

Shader* Shader::sendUniform(const std::string & name, const glm::vec2 & vector) {
	GLint location = getUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);
	return this;
}

Shader* Shader::sendUniform(const std::string & name, const glm::vec3 & vector) {
	GLint location = getUniformLocation(name);
	glUniform3f(location, vector.x, vector.y, vector.z);
	return this;
}

Shader* Shader::sendUniform(const std::string & name, const glm::vec4 & vector) {
	GLint location = getUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
	return this;
}

Shader* Shader::sendUniformMat3(const std::string & name, float * matrix, bool transpose) {
	GLint location = getUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
	return this;
}

Shader* Shader::sendUniformMat4(const std::string & name, float * matrix, bool transpose) {
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
	return this;
void Shader::sendUniform(const std::string& name, const glm::mat2 &matrix, bool transpose) {
	GLint location = getUniformLocation(name);
	glUniformMatrix2fv(location, 1, transpose, glm::value_ptr(matrix));
}
void Shader::sendUniform(const std::string& name, const glm::mat3 &matrix, bool transpose) {
	GLint location = getUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, glm::value_ptr(matrix));
}
void Shader::sendUniform(const std::string& name, const glm::mat4 &matrix, bool transpose) {
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, glm::value_ptr(matrix));
}

template<class T>
void Shader::sendUniform(const std::string& name, std::vector<T> vector) {
	for (int i = 0; i < vector.size(); i++) {
		Shader::sendUniform(name + "[" + std::to_string(i) + "]", vector[i]);
	}
}

std::string Shader::readFile(const std::string & fileName) const {
	std::ifstream input(fileName);

	if (!input) {
		std::cout << "Shader file not found: " << fileName << std::endl;
		return "";
	}

	std::string data(std::istreambuf_iterator<char>(input), (std::istreambuf_iterator<char>()));
	return data;
}

void Shader::outputShaderLog(GLuint shader) const {
	std::string infoLog;

	GLint infoLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

	infoLog.resize(infoLen);

	glGetShaderInfoLog(shader, infoLen, &infoLen, &infoLog[0]);

	std::cout << infoLog << std::endl;
}

void Shader::outputProgramLog() const {
	std::string infoLog;

	GLint infoLen;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);

	infoLog.resize(infoLen);

	glGetProgramInfoLog(program, infoLen, &infoLen, &infoLog[0]);

	std::cout << infoLog << std::endl;
}
