#pragma once

#include <GL\glew.h>
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool load(const std::string &file);
	Texture* unload();

	Texture* bind();
	Texture* unbind();

	// The handle to the texture object
	GLuint texObj = 0;
};