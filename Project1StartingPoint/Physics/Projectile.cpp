#include "Projectile.h"
#include <gdp/GameObject.h>

Projectile::Projectile()
{
}

Projectile::Projectile(float mass, glm::vec3 position)
	: position(position)
	, velocity(0.0f)
	, acceleration(0.0f)
	, damping(0.99f)
	, force(glm::vec3(0, 0, 0))
	, target(glm::vec3(0, 0, 0))
	, invMass(1.f/mass)
{

}

void Projectile::Shoot(float dt)
{
	acceleration = force * invMass;

	velocity += acceleration * dt;

	velocity *= damping;
	velocity += acceleration * dt;
	position += velocity * dt;

	if (position == target)
	{
		velocity = glm::vec3(0, 0, 0);
	}

}

void Projectile::AddForce(glm::vec3 dir)
{
	force += dir;
}

void Projectile::RemoveForce()
{
	force = glm::vec3(0.f, 0.f, 0.f);
}
