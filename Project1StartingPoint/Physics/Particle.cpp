#include "Particle.h"

Particle::Particle()
	: Particle(glm::vec3())
{

}

Particle::Particle(const glm::vec3& position)
	: position(position)
	, velocity(0.0f)
	, acceleration(0.0f)
	, damping(0.99f)
	, invMass(1.f)
	, radius(1.f)
	, force(glm::vec3(0,0,0))
	, m_IsStatic(false)
{

}

Particle::~Particle()
{

}

Particle::Particle(const Particle& particle)
{

}

Particle Particle::operator=(const Particle& particle)
{
	return Particle();
}

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

void Particle::SetPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

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

void Particle::ApplyForce(const glm::vec3& direction)
{
	force += direction;
}

void Particle::KillAllForces()
{
	force = glm::vec3(0.f, 0.f, 0.f);
}
