#pragma once

#include <glm/glm.hpp>
#include <gdp/GameObject.h>

class Projectile {
public:
	gdp::GameObject* gameObject;
	glm::vec3 velocity;
	glm::vec3 position;
	glm::vec3 acceleration;
	glm::vec3 target; 
	float damping = 0.99f;
	float invMass;
	glm::vec3 force;

	Projectile();
	Projectile(float mass, glm::vec3 position);

	void Shoot(float dt);
	void AddForce(glm::vec3);
	void RemoveForce();
};
