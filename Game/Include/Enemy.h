#pragma once

#include "Object.h"

class Enemy : public Object {
public:
	Enemy(glm::vec3 pos = { 0.f, 0.f, 0.f });
	~Enemy();

	virtual void update(float dt, Level* level);

	bool triggered = false;
	bool range = false;
	float attackSpeed = 0.5f; // time between attacks (in seconds)
	float movementSpeed; // amount to divide dt by
	float damage; // damage dealt to the player per attack
	float knockbackCD = 0.0f;


private:
	//void addMorphTarget(std::string filePath);
	//std::vector<std::shared_ptr<Mesh>> m_pMorphTargets;
	//int m_pCurrentKeyframe;
	//int m_pNextKeyframe;
	//float m_pLocalMorphTime; // the 't' value used for lerp between [0, 1]

};