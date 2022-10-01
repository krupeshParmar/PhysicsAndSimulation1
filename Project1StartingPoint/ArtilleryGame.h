#pragma once

#include <gdp/Engine.h>
#include <string>
#include "GameUnits.h"
#include "Physics/PhysicsSystem.h"
#include "Physics/Projectile.h"

using namespace gdp; 


// Projectile types used in the game
enum class ProjectileTypes
{
	ARMOR_PIERCING,						// AP	- Normal penetrating tank shell
	ARMOR_PIERCING_COMPOSITE_RIGID,		// APCR	- Super fast penetrating shell
	HIGH_EXPLOSIVES,					// HE	- High Explosives shell
	HIGH_EXPLOSIVE_ANTI_TANK,			// HEAT	- Explosive and penetrating shell
	HIGH_EXPLOSIVES_SQUASH_HEAD			// HESH - High explosive to damage tank crews
};

class ArtilleryGame {
public:
	ArtilleryGame();
	~ArtilleryGame();

	void Initialize();
	void Destroy();

	void StartNewGame();
	void GameUpdate();
	void DisplayTextToUser(const std::string& text);
	void ShootProjectile(glm::vec3);
	bool HitOrMiss();

private:
	GameObject* CreateGameObjectByType(const std::string& type);

	GameUnits m_PlayerTank;
	GameUnits m_EnemyTank;
	GameUnits m_Bullet;
	GameUnits m_AimBall;
	PhysicsSystem m_PhysicsSystem;
};