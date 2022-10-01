#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() {

}

PhysicsSystem::~PhysicsSystem() {
	for (int i = 0; i < m_Particles.size(); i++)
		delete m_Particles[i];
}

Particle* PhysicsSystem::CreateParticle(const glm::vec3& position)
{
	Particle* particle = new Particle(position);
	m_Particles.push_back(particle);
	return particle;
}

void PhysicsSystem::UpdateStep(float duration, bool isProjectile)
{
	size_t numParticles = m_Particles.size();
	for (int i = 0; i < numParticles; i++)
	{
		m_Particles[i]->ApplyForce(glm::vec3(0, -9.81f, 0));
	}

	for (int i = 0; i < numParticles; i++)
	{
		m_Particles[i]->Integrate(duration);
	}

	/*if (isProjectile)
		return;*/

	for (int i = 0; i < numParticles; i++)
	{
		m_Particles[i]->KillAllForces();
	}
}
