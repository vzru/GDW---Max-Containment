#include "ParticleEmitterSoA.h"
#include <algorithm>
#include "Material.h"
#include "Shader.h"
#include "Camera.h"
#include "glm/gtc/random.hpp"

#include <glm\gtc\type_ptr.hpp>

ParticleEmitterSoA::ParticleEmitterSoA()
{
	// Initialize memory pointers to null
	particles.positions = nullptr;
	particles.velocities = nullptr;
	particles.remainingLives = nullptr;
	particles.accelerations = nullptr;
	particles.masses = nullptr;
	numParticles = 0;

	playing = false;
}

ParticleEmitterSoA::~ParticleEmitterSoA()
{
	freeMemory();
}

void ParticleEmitterSoA::init(int numP)
{
	if (allocated)
		freeMemory();

	if (numP) // make sure new array size is not 0
	{
		particles.positions = new glm::vec3[numP];
		particles.velocities = new glm::vec3[numP];
		particles.accelerations = new glm::vec3[numP];
		particles.remainingLives = new float[numP];
		particles.masses = new float[numP];
		memset(particles.remainingLives, 0, sizeof(float) * numP);

		numParticles = numP;
		allocated = true; // mark that memory has been allocated

						  // Set up VBO
						  // **** LOOK HERE ****
		AttributeDescriptor posAttrib;
		posAttrib.attributeLocation = VERTEX;
		posAttrib.data = particles.positions; // data points to particles.positions
		posAttrib.elementSize = sizeof(float);
		posAttrib.elementType = GL_FLOAT;
		posAttrib.numElements = numP * 3;
		posAttrib.numElementsPerAttrib = 3;

		vbo.addAttributeArray(posAttrib);

		vbo.primitiveType = GL_POINTS;

		vbo.createVBO(GL_DYNAMIC_DRAW);
	}
}

void ParticleEmitterSoA::update(float dt)
{
	if (allocated && playing)
	{
		// loop through each particle
		for (int i = 0; i < numParticles; i++)
		{
			// Get each property for the particle using pointer arithmetic
			glm::vec3* pos = particles.positions + i;
			glm::vec3* vel = particles.velocities + i;
			glm::vec3* accel = particles.accelerations + i;
			float* life = particles.remainingLives + i;
			float* mass = particles.masses + i;
			// other properties... 

			// check if alive
			if (*life <= 0)
			{
				// if dead respawn
				// could put additional logic here...
				*pos = initPos;

				(*vel).x = glm::mix(initForceMin.x, initForceMax.x, glm::linearRand(0.0f, 1.0f));
				(*vel).y = glm::mix(initForceMin.y, initForceMax.y, glm::linearRand(0.0f, 1.0f));
				(*vel).z = glm::mix(initForceMin.z, initForceMax.z, glm::linearRand(0.0f, 1.0f));

				*life = glm::linearRand(lifeR.x, lifeR.y);
				*mass = glm::linearRand(0.5f, 1.0f);
				*accel = *vel / *mass;
			}

			// Update position and velocity
			*pos += *vel * dt + *accel * 0.5f * (dt*dt);
			*vel += dt;
			*life -= dt;
		}
	}
}

void ParticleEmitterSoA::draw(Shader *shader, Camera *cam)
{
	AttributeDescriptor* attrib = vbo.getAttributeDescriptor(VERTEX);

	glBindVertexArray(vbo.getVAO());

	glEnableVertexAttribArray(attrib->attributeLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO(VERTEX));
	glBufferSubData(GL_ARRAY_BUFFER, 0, attrib->numElements * attrib->elementSize, attrib->data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader->bind();
	shader->sendUniformMat4("u_mvp", glm::value_ptr(transform), false);
	shader->sendUniformMat4("u_mv", glm::value_ptr(cam->getView()), false);
	shader->sendUniformMat4("u_proj", glm::value_ptr(cam->getProj()), false);
	//shader->sendUniformMat4["u_mvp"] = cam->viewProjMatrix;
	//shader->sendUniformMat4["u_mv"] = cam->viewMatrix;
	//shader->sendUniformMat4["u_proj"] = cam->projMatrix;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);

	vbo.draw();

	glDepthMask(true);

}

void ParticleEmitterSoA::freeMemory()
{
	// Free up all arrays here
	if (allocated)
	{
		delete[] particles.positions;
		delete[] particles.velocities;
		delete[] particles.remainingLives;
		delete[] particles.accelerations;
		delete[] particles.masses;

		particles.positions = nullptr;
		particles.velocities = nullptr;
		particles.remainingLives = nullptr;
		particles.accelerations = nullptr;
		particles.masses = nullptr;

		allocated = false;
		numParticles = 0;
	}
}
