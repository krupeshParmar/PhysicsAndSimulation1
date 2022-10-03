#include "Particle.h"

Particle::Particle()
	: Particle(glm::vec3())
{

}

// Create particle with given values
// Note: every values needs to be initialized or project breaks
Particle::Particle(const glm::vec3& position)
	: position(position)
	, velocity(0.0f)
	, acceleration(0.0f)
	, damping(0.99f)
	, invMass(1.f)
	, radius(1.f)
	, force(glm::vec3(0,0,0))
	, m_IsStatic(false)
	, isBullet(false)
{

}

Particle::~Particle()
{

}

Particle::Particle(const Particle& particle)
{

}

// Assign particle
Particle Particle::operator=(const Particle& particle)
{
	return Particle();
}

// Set mass of the particle
void Particle::SetMass(float mass)
{
	if (mass <= 0)
	{
		m_IsStatic = true;
		invMass = -1.f;
	}
	invMass = 1.f / mass;
	m_IsStatic = false;
}

// Set position of the particle
void Particle::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

// Integrate force in particle and update position
void Particle::Integrate(float dt)
{
	if (invMass <= 0 || m_IsStatic)
		return;

	acceleration = force * invMass;
	velocity += acceleration * dt;

	velocity *= damping;
	position += velocity * dt;

	if (position.y <= radius)
	{
		position.y = radius;
		velocity.y = abs(velocity.y);
	}
}

// Apply force to particle in given direction
void Particle::ApplyForce(const glm::vec3& direction)
{
	force += direction;
}

// Kill all the forces
void Particle::KillAllForces()
{
	force = glm::vec3(0.f, 0.f, 0.f);
}
