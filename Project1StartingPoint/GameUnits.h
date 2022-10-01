#pragma once

#include "Physics/Particle.h"
#include <gdp/GameObject.h>

class GameUnits {
public:
	GameUnits();
	~GameUnits();

	Particle* particle;
	gdp::GameObject* gameObject;
};