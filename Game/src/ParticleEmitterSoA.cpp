#include "ParticleEmitterSoA.h"
#include <algorithm>
#include "Material.h"
#include "Shader.h"
#include "Camera.h"
#include "glm/gtc/random.hpp"

#include <glm\gtc\type_ptr.hpp>

ParticleEmitterSoA::ParticleEmitterSoA() {
	// Initialize memory pointers to null
	particles.positions = nullptr;
	particles.velocities = nullptr;
	particles.remainingLives = nullptr;
	particles.accelerations = nullptr;
	particles.masses = nullptr;
	numParticles = 0;

	playing = false;
}

ParticleEmitterSoA::~ParticleEmitterSoA() {
	freeMemory();
}

void ParticleEmitterSoA::init(int numP) {
	if (allocated)
		freeMemory();

	if (numP) { // make sure new array size is not 0
		particles.positions = new glm::vec3[numP];
		particles.velocities = new glm::vec3[numP];
		particles.accelerations = new glm::vec3[numP];
		particles.remainingLives = new float[numP];
		particles.masses = new float[numP];
		memset(particles.remainingLives, 0, sizeof(float) * numP);

		numParticles = numP;
		allocated = true; // mark that memory has been allocated

		// Set up VBO
		AttributeDescriptor posAttrib;
		posAttrib.attributeLocation = AttributeLocations::VERTEX;
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

void ParticleEmitterSoA::update(float dTime) {
	if (allocated /*&& playing*/) {
		// loop through each particle
		for (int i = 0; i < numParticles; i++) 		{
			// Get each property for the particle using pointer arithmetic
			glm::vec3* pos = particles.positions + i;
			glm::vec3* vel = particles.velocities + i;
			glm::vec3* accel = particles.accelerations + i;
			float* life = particles.remainingLives + i;
			float* mass = particles.masses + i;
			// other properties... 

			// check if alive
			if (*life <= 0) {
				// if dead respawn
				// could put additional logic here...
				if (playing ) {
					*pos = initPos;

					(*vel).x = glm::mix(initForceMin.x, initForceMax.x, glm::linearRand(0.0f, 1.0f));
					(*vel).y = glm::mix(initForceMin.y, initForceMax.y, glm::linearRand(0.0f, 1.0f));
					(*vel).z = glm::mix(initForceMin.z, initForceMax.z, glm::linearRand(0.0f, 1.0f));

					*life = glm::linearRand(lifeR.x, lifeR.y);
					*mass = glm::linearRand(0.5f, 1.0f);
					*accel = *vel / *mass;
				}
			}

			float dt = dTime / dtFactor;
			// Update position and velocity
			*pos += *vel * dt + *accel * 2.0f * (dt * dt);
			*vel += dt * *vel;
			*life -= dt;
		}
	}
}

void ParticleEmitterSoA::draw(Shader *shader, Camera *cam) {
	AttributeDescriptor* attrib = vbo.getAttributeDescriptor(VERTEX);

	glBindVertexArray(vbo.getVAO());

	glEnableVertexAttribArray(attrib->attributeLocation);
	glBindBuffer(GL_ARRAY_BUFFER, vbo.getVBO(VERTEX));
	glBufferSubData(GL_ARRAY_BUFFER, 0, attrib->numElements * attrib->elementSize, attrib->data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader->bind();
	shader->sendUniform("u_model", transform);
	shader->sendUniform("u_view", cam->getView());
	shader->sendUniform("u_proj", cam->getProj());
	shader->sendUniform("u_color", color);
	shader->sendUniform("size", size);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(false);

	vbo.draw();

	glDepthMask(true);
}

void ParticleEmitterSoA::freeMemory() {
	// Free up all arrays here
	if (allocated) {
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
