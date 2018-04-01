#pragma once

#include <GL\glew.h>
#include <string>

class Texture
{
public:
	Texture();
	~Texture();

	bool load(const std::string &file);
	void unload();

	void bind(GLenum unit);
	void unbind(GLenum unit);

	// The handle to the texture object
	GLuint texObj = 0;
};