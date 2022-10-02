#pragma once

#include <glm/glm.hpp>

class Particle {

public:
	Particle();
	Particle(const glm::vec3& position);
	Particle(const Particle& particle);
	~Particle();

	Particle operator=(const Particle& particle);

	void SetMass(float mass);

	void Integrate(float dt);
	void ApplyForce(const glm::vec3& direction);
	void KillAllForces();

	void SetPosition(glm::vec3 newPosition);

	inline const glm::vec3& GetPosition() const { return position; }
	inline const glm::vec3& GetVelocity() const { return velocity; }
	inline const glm::vec3& GetAcceleration() const { return acceleration; }

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 force;
	float radius;
	float damping;
	bool isBullet;

private:
	float invMass;
	bool m_IsStatic;
};