#pragma once

#include "VertexBufferObject.h"
#include "GLM/vec3.hpp"
#include "Object.h"
#include <memory>
#include <math.h>
#include "Material.h"

class ParticleEmitterSoA : public Object
{
	struct Particle
	{
		glm::vec3* positions;
		glm::vec3* velocities;
		glm::vec3* accelerations;
		float* remainingLives;
		float* masses;
	} particles;

public:
	ParticleEmitterSoA();
	~ParticleEmitterSoA();

	void init(int numP);
	void update(float dt);
	void draw(Shader *shader, Camera* cam);

	inline void play() { playing = true; }
	inline void pause() { playing = false; }

	void freeMemory();

	glm::vec3 initPos;
	glm::vec3 lifeR;
	glm::vec3 initForceMin;
	glm::vec3 initForceMax;

	//glm::vec4 direction;

	VertexBufferObject vbo;

	bool playing;

	std::shared_ptr<Material> material;

private:
	int numParticles;
	bool allocated;
};