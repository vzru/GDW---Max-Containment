#include "Mesh.h"
#include "Enemy.h"
#include "Material.h"

Enemy::Enemy(glm::vec3 pos) : Object(pos) {
	//mat = new Material();
	//mesh->load("assets/models/enemy.obj");
	life = 10.f;
}
Enemy::~Enemy() {}

void Enemy::update(float dt, Level* level) {
	collide(dt, level, true);

	position += velocity * (dt / moveSpeed);

	// player is invincible
	if (cooldown > 0.f)
		cooldown -= dt / 1000.f;

	Object::update(dt);
}
//
//void Enemy::addMorphTarget(std::string filePath)
//{
//	std::shared_ptr<Mesh> newTarget = std::make_shared<Mesh>();
//	newTarget->load(filePath);
//	// Initialize the mesh object in the base class
//	if (!mesh)
//	{
//		// Allocate memory if this is the first target being added
//		// The mesh object in the base class is where we will store the intermediate morph targets
//		// and is what will actually end up on screen
//		mesh = std::make_shared<Mesh>();
//		int numVerts = newTarget->vertexData.size();
//		mesh->vertexData.resize(numVerts);
//		mesh->normalData.resize(numVerts);
//		mesh->textureData.resize(numVerts);
//	}
//	else
//	{
//		// Remember: in order for morphing to work correctly each target must have the same number of vertices
//		// We will perform a check to make sure the new target has the same number of vertices as the existing ones
//
//		if (mesh->vertices.size() != newTarget->vertices.size())
//		{
//			std::cout << "New target does not have the same number of vertices as previous target!" << std::endl;
//			return;
//		}
//	}
//
//	// Store it in the list of morph targets
//	m_pMorphTargets.push_back(newTarget);
//}
