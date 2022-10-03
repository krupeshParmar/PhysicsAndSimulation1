#include "PhysicsSystem.h"
#include "../GameUnits.h"

PhysicsSystem::PhysicsSystem() {

}

PhysicsSystem::~PhysicsSystem() {
	for (int i = 0; i < m_Particles.size(); i++)
		delete m_Particles[i];
}


// Create particle on given position
Particle* PhysicsSystem::CreateParticle(const glm::vec3& position)
{
	Particle* particle = new Particle(position);
	m_Particles.push_back(particle);
	return particle;
}

// Update particles each frame
void PhysicsSystem::UpdateStep(float duration)
{
	size_t numParticles = m_Particles.size();



	for (int i = 0; i < numParticles; i++)
	{
		m_Particles[i]->ApplyForce(glm::vec3(0, -9.81f, 0));
	}

	for (int i = 0; i < numParticles; i++)
	{
		if (m_Particles[i]->isBullet)
			continue;
		m_Particles[i]->Integrate(duration);
	}

	for (int i = 0; i < numParticles; i++)
	{
		m_Particles[i]->KillAllForces();
	}
}
