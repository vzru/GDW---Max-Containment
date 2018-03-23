#include <fstream>
#include <iostream>
#include <vector>

#include "Shader.h"

Shader::Shader() {}

Shader::~Shader() {
	if (loaded)	unload();
}

bool Shader::load(const std::string &vertFile, const std::string &fragFile) {
	// create shader and program objects
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

	// load our source code
	std::string source = readFile(vertFile);
	const GLchar *temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(vertexShader, 1, &temp, NULL);

	source = readFile(fragFile);
	temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(fragShader, 1, &temp, NULL);

	// Compile Code
	std::cout << "Loading shader: " << vertFile << std::endl;
	if (!compileShader(vertexShader)) {
		std::cout << "Vertex shader failed to compile." << std::endl;
		outputShaderLog(vertexShader);
		unload();
		system("pause");
		return false;
	}

	std::cout << "Loading shader: " << fragFile << std::endl;
	if (!compileShader(fragShader)) {
		std::cout << "Fragment shader failed to compile." << std::endl;
		outputShaderLog(fragShader);
		unload();
		system("pause");
		return false;
	}

	// Setup program object
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);

	if (!linkProgram()) {
		std::cout << "Shader program failed to link." << std::endl;
		outputProgramLog();
		unload();
		return false;
	}

	loaded = true;
	return true;
}

bool Shader::load(const std::string & vertFile, const std::string & geoFile, const std::string & fragFile)
{
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	geoShader = glCreateShader(GL_GEOMETRY_SHADER);
	fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	program = glCreateProgram();

	// load our source code
	std::string source = readFile(vertFile);
	const GLchar *temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(vertexShader, 1, &temp, NULL);

	source = readFile(geoFile);
	temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(geoShader, 1, &temp, NULL);

	source = readFile(fragFile);
	temp = static_cast<const GLchar*>(source.c_str());
	glShaderSource(fragShader, 1, &temp, NULL);

	// Compile Code
	std::cout << "Loading shader: " << vertFile << std::endl;
	if (!compileShader(vertexShader)) {
		std::cout << "Vertex shader failed to compile." << std::endl;
		outputShaderLog(vertexShader);
		unload();
		system("pause");
		return false;
	}

	std::cout << "Loading shader: " << geoFile << std::endl;
	if (!compileShader(geoShader)) {
		std::cout << "Geometry shader failed to compile." << std::endl;
		outputShaderLog(geoShader);
		unload();
		system("pause");
		return false;
	}

	std::cout << "Loading shader: " << fragFile << std::endl;
	if (!compileShader(fragShader)) {
		std::cout << "Fragment shader failed to compile." << std::endl;
		outputShaderLog(fragShader);
		unload();
		system("pause");
		return false;
	}

	// Setup program object
	glAttachShader(program, vertexShader);
	glAttachShader(program, geoShader);
	glAttachShader(program, fragShader);

	if (!linkProgram()) {
		std::cout << "Shader program failed to link." << std::endl;
		outputProgramLog();
		unload();
		return false;
	}

	loaded = true;
	return true;
}

bool Shader::isLoaded() const {
	return loaded;
}

void Shader::unload() {
	if (vertexShader != 0) {
		glDetachShader(program, vertexShader);
		glDeleteShader(vertexShader);
		vertexShader = 0;
	}
	if (geoShader != 0) {
		glDetachShader(program, geoShader);
		glDeleteShader(geoShader);
		geoShader = 0;
	}
	if (fragShader != 0) {
		glDetachShader(program, fragShader);
		glDeleteShader(fragShader);
		fragShader = 0;
	}
	if (program != 0) {
		glDeleteProgram(program);
		program = 0;
	}

	loaded = false;
}

bool Shader::linkProgram() {
	glLinkProgram(program);

	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	return success == GL_TRUE;
}

void Shader::bind() const {
	glUseProgram(program);
}

void Shader::unbind() {
	glUseProgram(GL_NONE);
}

void Shader::addAttribute(unsigned int index, const std::string & attribName) {
	glBindAttribLocation(program, index, attribName.c_str());
}

int Shader::getAttribLocation(const std::string & attribName) {
	return glGetAttribLocation(program, attribName.c_str());
}

int Shader::getUniformLocation(const std::string & uniformName) {
	return glGetUniformLocation(program, uniformName.c_str());
}

void Shader::sendUniform(const std::string & name, int integer) {
	GLint location = getUniformLocation(name);
	glUniform1i(location, integer);
}

void Shader::sendUniform(const std::string & name, unsigned int unsignedInteger) {
	GLint location = getUniformLocation(name);
	glUniform1ui(location, unsignedInteger);
}

void Shader::sendUniform(const std::string & name, float scalar) {
	GLint location = getUniformLocation(name);
	glUniform1f(location, scalar);
}

void Shader::sendUniform(const std::string & name, const glm::vec2 & vector) {
	GLint location = getUniformLocation(name);
	glUniform2f(location, vector.x, vector.y);
}

void Shader::sendUniform(const std::string & name, const glm::vec3 & vector) {
	GLint location = getUniformLocation(name);
	glUniform3f(location, vector.x, vector.y, vector.z);
}

void Shader::sendUniform(const std::string & name, const glm::vec4 & vector) {
	GLint location = getUniformLocation(name);
	glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
}

void Shader::sendUniformMat3(const std::string & name, float * matrix, bool transpose) {
	GLint location = getUniformLocation(name);
	glUniformMatrix3fv(location, 1, transpose, matrix);
}

void Shader::sendUniformMat4(const std::string & name, float * matrix, bool transpose) {
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix);
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

bool Shader::compileShader(GLuint shader) const {
	glCompileShader(shader);

	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	return success == GL_TRUE;
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
