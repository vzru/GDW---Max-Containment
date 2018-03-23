#include "VertexBufferObject.h"
#include <iostream>

VertexBufferObject::VertexBufferObject()
{
	vaoHandle = 0;
	primitiveType = GL_TRIANGLES;
}

VertexBufferObject::~VertexBufferObject()
{
	destroy();
}

int VertexBufferObject::addAttributeArray(AttributeDescriptor attrib)
{
	attributeDescriptors.push_back(attrib);
	return 1;
}

AttributeDescriptor* VertexBufferObject::getAttributeDescriptor(AttributeLocations loc)
{
	for (int i = 0; i < attributeDescriptors.size(); i++)
	{
		if (attributeDescriptors[i].attributeLocation == loc)
			return &attributeDescriptors[i];
	}
	return nullptr;
}

unsigned int VertexBufferObject::getVAO()
{
	return vaoHandle;
}

unsigned int VertexBufferObject::getVBO(AttributeLocations loc)
{
	for (int i = 0; i < attributeDescriptors.size(); i++)
	{
		if (attributeDescriptors[i].attributeLocation == loc)
			return vboHandles[i];
	}
	return 0;
}

void VertexBufferObject::createVBO(GLenum vboUsage)
{
	if (vaoHandle)
	{
		destroy();
	}

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	auto numBuffers = attributeDescriptors.size();
	vboHandles.resize(numBuffers);

	glGenBuffers(numBuffers, &vboHandles[0]);

	for (unsigned int i = 0; i < numBuffers; i++)
	{
		AttributeDescriptor* attrib = &attributeDescriptors[i];
		
		glEnableVertexAttribArray(attrib->attributeLocation);
		glBindBuffer(GL_ARRAY_BUFFER, vboHandles[i]);
		glBufferData(GL_ARRAY_BUFFER, attrib->numElements * attrib->elementSize,
			attrib->data, vboUsage);

		glVertexAttribPointer(attrib->attributeLocation, attrib->numElementsPerAttrib,
			attrib->elementType, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(0);
}

void VertexBufferObject::draw()
{
	if (vaoHandle)
	{
		glBindVertexArray(vaoHandle);
		// better way would be to just store the num of vertices
		glDrawArrays(primitiveType, 0,
			attributeDescriptors[0].numElements / attributeDescriptors[0].numElementsPerAttrib);

		glBindVertexArray(0);
	}
}

void VertexBufferObject::destroy()
{
	if (vaoHandle)
	{
		glDeleteVertexArrays(1, &vaoHandle);
		glDeleteBuffers((GLsizei)vboHandles.size(), &vboHandles[0]);
	}

	vboHandles.clear();
	attributeDescriptors.clear();
}


