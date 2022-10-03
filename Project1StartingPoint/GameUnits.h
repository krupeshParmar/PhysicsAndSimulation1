#pragma once

#include "Physics/Particle.h"
#include <gdp/GameObject.h>
#include <vector>

// Container for all the game units
class GameUnits {
public:
	GameUnits();
	~GameUnits();

	Particle* particle;
	gdp::GameObject* gameObject;
};