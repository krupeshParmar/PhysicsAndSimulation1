#pragma once

#include "Particle.h"

#include <vector>

class PhysicsSystem
{
public:
	PhysicsSystem();
	~PhysicsSystem();

	Particle* CreateParticle(const glm::vec3& position);
	void UpdateStep(float duration, bool, glm::vec3);

private:
	std::vector<Particle*> m_Particles;
};