#pragma once

#include "GL/glew.h"
#include <vector>

// These locations correspond to the locations in shaders (glEnableVertexAttribArray)
// These are constants and will never change
// A VBO may have more attribute locations, but these ones never change
// It's a good idea to use the same attribute location layouts for each shader,
// Not required, but it makes VBO creation easier
enum AttributeLocations
{
	VERTEX = 0,
	NORMAL,
	TEX_COORD,
	COLOUR
};

// This struct describes the array for an attribute
struct AttributeDescriptor
{
public:
	AttributeDescriptor()
	{
		elementType = GL_FLOAT;
		elementSize = 0;
		numElements = 0;
		attributeLocation = AttributeLocations::VERTEX;
		attributeName = "";
		data = nullptr;
	}

	AttributeLocations attributeLocation;

	GLenum elementType;					// Type of the elements in array (ie GL_FLOAT)
	unsigned int elementSize;			// Size (in bytes) of each element in array
	unsigned int numElementsPerAttrib;  // Number of elements for one attribute (ie a vertex position has 3)
	unsigned int numElements;			// Number of elements in entire array
	std::string attributeName;			// Name of the attribute as it appears in the shader
	void* data;							// Pointer to data
};

class VertexBufferObject
{
private:
	// Pointer to the arrays of data
	std::vector<AttributeDescriptor> attributeDescriptors;

	// VAO keeps track of everything we do to a VBO
	// This is useful because when it comes time to use the VBO,
	// we do not have to set all of the options.
	// We just use the VAO
	unsigned int vaoHandle;

	// There can be multiple arrays of data used by a single VBO
	// ie. a buffer for normals, vertices, texture coords etc.
	// OR (ideally) there will only be one array where each attribute
	// is interleaved. 
	std::vector<unsigned int> vboHandles;

public:
	VertexBufferObject();
	~VertexBufferObject();

	// Pass in AttributeDescriptor descriptors for each attribute
	// this object will have.
	int addAttributeArray(AttributeDescriptor attrib);

	// Returns a pointer to the attribute descriptor for the specified location
	AttributeDescriptor* getAttributeDescriptor(AttributeLocations loc);

	// Returns the VAO handle
	unsigned int getVAO();

	// Returns the handle for the specified VBO
	unsigned int getVBO(AttributeLocations loc);


	// Call this once you add all the AttributeDescriptor objects
	void createVBO(GLenum vboUsage);

	// Call this when you want to draw the object
	void draw();

	// Call this when you want to destroy the object
	// Tip: Might want to put this in the destructor  
	void destroy();

	GLenum primitiveType; // defaults to GL_TRIANGLES
};
