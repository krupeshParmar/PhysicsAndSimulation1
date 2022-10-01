#include "ArtilleryGame.h"
#include <iostream>
#include "AssetInfo.h"
#include <random>
#include <math.h>


#define AP glm::vec3(0.3, 0.8, 1)
#define APCR glm::vec3(0, 0.5, 1)
#define HE glm::vec3(1, 0.6, 0.35)
#define HEAT glm::vec3(1, 0.8, 0.1)
#define HESH glm::vec3(1, 0.25, 0)
#define DEBUG_LOG_ENABLED
#ifdef DEBUG_LOG_ENABLED
#define DEBUG_PRINT(x, ...) printf(x, __VA_ARGS__)
#else
#define DEBUG_PRINT(x)
#endif


// Ids preloaded in the main.cpp to be referenced when
// creating specific game objects for your game.
// - EnemyTank, PlayerTank, and Bullet
unsigned int TankModelId;
unsigned int ProjectileModelId;
unsigned int PlayerMaterialId;
unsigned int EnemyMaterialId;
unsigned int BulletMaterialId;

unsigned int SphereMaterialId_AP = -10;
unsigned int SphereMaterialId_APCR = -11;
unsigned int SphereMaterialId_HE = -12;
unsigned int SphereMaterialId_HEAT = -13;
unsigned int SphereMaterialId_HESH = -14;
unsigned int SphereModelId = -10;
unsigned int SphereTextureid = -10;

glm::vec3 dirToShoot;

bool IF_SHOT = false;
bool IF_HIT = false;
bool IF_MISS = false;
ProjectileTypes projectileType;

glm::vec3 RandomVector(int max, int min)
{
	int x = rand() % (max * 2 +1) + min;
	int z = rand() % (max * 2 + 1) + min;
	DEBUG_PRINT("x= %d, z = %d\n", x, z);
	return glm::vec3(x, 10.f, z);
}

glm::vec3 NormalizeVector(glm::vec3 vec)
{
	float maxValue = 40.f;
	return vec - (-20.f) / maxValue;
}

float AngleBetweenVectors(glm::vec3 vec1, glm::vec3 vec2)
{
	vec1 = NormalizeVector(vec1);
	vec2 = NormalizeVector(vec2);
	float dotProduct = vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
	float det1 = std::sqrt((vec1.x * vec1.x) + (vec1.y * vec1.y) + (vec1.z * vec1.z));
	float det2 = std::sqrt((vec2.x * vec2.x) + (vec2.y * vec2.y) + (vec2.z * vec2.z));
	float angle = dotProduct / (det1 * det2);
	//angle = glm::radians(angle);
	return acos(angle);
}

void ArtilleryGame::ShootProjectile(glm::vec3 direction)
{
	/*if (m_Bullet.particle->GetPosition() == direction)
	{
		m_Bullet.particle->velocity = glm::vec3(0.f, 0.f, 0.f);
		return;

		         ------------------------------
		---------------------------------------

	}*/

	switch (projectileType)
	{
	case ProjectileTypes::ARMOR_PIERCING:
		m_Bullet.particle->SetMass(10.f);
		m_Bullet.particle->ApplyForce(direction);
		break;
	case ProjectileTypes::ARMOR_PIERCING_COMPOSITE_RIGID:
		m_Bullet.particle->SetMass(5.f);
		m_Bullet.particle->ApplyForce(direction);
		break;
	case ProjectileTypes::HIGH_EXPLOSIVES:
		m_Bullet.particle->SetMass(20.f);
		m_Bullet.particle->ApplyForce(direction);
		break;
	case ProjectileTypes::HIGH_EXPLOSIVE_ANTI_TANK:
		m_Bullet.particle->SetMass(15.f);
		m_Bullet.particle->ApplyForce(direction);
		break;
	case ProjectileTypes::HIGH_EXPLOSIVES_SQUASH_HEAD:
		m_Bullet.particle->SetMass(30.f);
		m_Bullet.particle->ApplyForce(direction);
		break;
	default:
		break;
	}
}

bool ArtilleryGame::HitOrMiss()
{
	if (m_Bullet.particle->position.x <= m_EnemyTank.particle->position.x + 1 && 
		m_Bullet.particle->position.x >= m_EnemyTank.particle->position.x - 1
		&& m_Bullet.particle->position.z <= m_EnemyTank.particle->position.z + 1
		&& m_Bullet.particle->position.z >= m_EnemyTank.particle->position.z - 1)
	{
		m_Bullet.particle->SetMass(-1.f);
		m_Bullet.particle->SetPosition(m_EnemyTank.particle->position);
		return true;
	}

	if (m_Bullet.particle->position.x <= m_AimBall.particle->position.x + 1 &&
		m_Bullet.particle->position.x >= m_AimBall.particle->position.x - 1
		&& m_Bullet.particle->position.z <= m_AimBall.particle->position.z + 1
		&& m_Bullet.particle->position.z >= m_AimBall.particle->position.z - 1)
	{
		if(!IF_MISS)
			std::cout << "ENEMY TANK MISSED :( \t Press 'N' to start new Game" << std::endl;
		IF_MISS = true;
		m_Bullet.particle->SetMass(-1.f);
		m_Bullet.particle->SetPosition(m_AimBall.particle->position);
	}
	return false;
}

// TODO:
// #include "YourPhysicsClass.h"

/// <summary>
/// Default constructor
/// Set all class variables to the desired default state
/// </summary>
ArtilleryGame::ArtilleryGame()
	/*: m_PlayerTank(nullptr)
	, m_EnemyTank(nullptr)
	, m_Bullet(nullptr) */
{
	DEBUG_PRINT("ArtilleryGame: ArtilleryGame\n");
}

/// <summary>
/// Default destructor
/// </summary>
ArtilleryGame::~ArtilleryGame()
{
	DEBUG_PRINT("ArtilleryGame::~ArtilleryGame\n");
}
/// <summary>
/// Create everything needed for your game here
/// such as memory allocation calls
/// </summary>
void ArtilleryGame::Initialize()
{
	srand(time(0));
	// TODO:
	// - Create a Player Tank GameObject
	// - Create an Enemy Tank GameObject
	// - Create projectile(s)
	projectileType = ProjectileTypes::ARMOR_PIERCING;
	m_PlayerTank.particle = m_PhysicsSystem.CreateParticle(RandomVector(20.f, -20.f));
	m_PlayerTank.gameObject = CreateGameObjectByType("Player");

	m_EnemyTank.particle = m_PhysicsSystem.CreateParticle(RandomVector(20.f, -20.f));
	m_EnemyTank.gameObject = CreateGameObjectByType("Enemy");

	m_Bullet.particle = m_PhysicsSystem.CreateParticle(m_PlayerTank.particle->position);
	m_Bullet.gameObject = CreateGameObjectByType("Bullet");
	m_Bullet.particle->radius = 0.7f;
	//m_Bullet.gameObject->Renderer.MaterialId = SphereMaterialId_AP;


	GDP_LoadModel(SphereModelId, "assets/models/sphere.obj");
	GDP_LoadTexture(SphereTextureid, "assets/textures/white.png");

	GDP_CreateMaterial(SphereMaterialId_AP, SphereTextureid, AP);
	GDP_CreateMaterial(SphereMaterialId_APCR, SphereTextureid, APCR);
	GDP_CreateMaterial(SphereMaterialId_HE, SphereTextureid, HE);
	GDP_CreateMaterial(SphereMaterialId_HEAT, SphereTextureid, HEAT);
	GDP_CreateMaterial(SphereMaterialId_HESH, SphereTextureid, HESH);

	m_AimBall.particle = m_PhysicsSystem.CreateParticle(glm::vec3(0, 10.f, 0));
	m_AimBall.gameObject = GDP_CreateGameObject();
	m_AimBall.gameObject->Renderer.ShaderId = 1;
	m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_AP;
	m_AimBall.gameObject->Renderer.MeshId = SphereModelId;
	m_AimBall.gameObject->Scale = glm::vec3(0.7, 0.7, 0.7);

	ArtilleryGame::StartNewGame();
}

/// <summary>
/// Handle everything that needs to be destroyed
/// such as memory de-allocation.
/// </summary>
void ArtilleryGame::Destroy()
{
	DEBUG_PRINT("ArtilleryGame::Destroy\n");
	// TODO:
}

/// <summary>
/// StartNewGame
/// 1. Randomly place the enemy tank at a random valid location 
///    on the map
/// 2. Randomly place the player tank at a random valid location
///    on the map
/// 3. Reset the bullet(s)
/// </summary>
void ArtilleryGame::StartNewGame()
{
	//DEBUG_PRINT("ArtilleryGame::StartNewGame\n");
	// TODO:
	IF_SHOT = false;
	m_EnemyTank.particle->SetPosition(RandomVector(20, -20));
	m_PlayerTank.particle->SetPosition(RandomVector(20, -20));
	m_Bullet.particle->SetPosition(m_PlayerTank.particle->GetPosition());
	m_Bullet.particle->SetMass(1.f);
}

/// <summary>
/// Update all bullets here
/// - Call your Physics class to update all projectiles
/// Check for bullet collision (position.y <= 0, hit ground)
/// Check for User input:
/// >>> GDP_IsKeyPressed, GDP_IsKeyHeldDown <-- Case Sensitive
/// >>> example: if (GDP_IsKeyPressed('n') || GDP_IsKeyPressed('N'))
/// - Input for changing the bullet direction	(wasd, etc)
/// - Input to change the projectile type		(1,2,3,4,5)
/// - Input to fire a projectile				(space)
/// - Input to start a new game					(n)
/// </summary>
void ArtilleryGame::GameUpdate()
{

	// TODO:
	if (GDP_IsKeyPressed('n'))
	{
		ArtilleryGame::StartNewGame();
	}

	if (HitOrMiss() && !IF_HIT)
	{
		IF_HIT = true;
		std::cout << "ENEMY TANK HIT :) \t Press 'N' to start new game!";
	}

	if (!IF_SHOT)
	{
		dirToShoot = m_AimBall.particle->position - m_Bullet.particle->position;
		if (GDP_IsKeyHeldDown('a'))
			m_AimBall.particle->ApplyForce(glm::vec3(1, 0, 0));

		if (GDP_IsKeyHeldDown('d'))
			m_AimBall.particle->ApplyForce(glm::vec3(-1, 0, 0));

		if (GDP_IsKeyHeldDown('w'))
			m_AimBall.particle->ApplyForce(glm::vec3(0, 0, 1));

		if (GDP_IsKeyHeldDown('s'))
			m_AimBall.particle->ApplyForce(glm::vec3(0, 0, -1));

		if (GDP_IsKeyHeldDown('1')) {
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_AP;
			projectileType = ProjectileTypes::ARMOR_PIERCING;
			//m_Bullet.gameObject->Renderer.MaterialId = SphereMaterialId_AP;
		}

		if (GDP_IsKeyHeldDown('2')) {
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_APCR;
			projectileType = ProjectileTypes::ARMOR_PIERCING_COMPOSITE_RIGID;
			//m_Bullet.gameObject->Renderer.MaterialId = SphereMaterialId_APCR;
		}

		if (GDP_IsKeyHeldDown('3')) {
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_HE;
			projectileType = ProjectileTypes::HIGH_EXPLOSIVES;
			//m_Bullet.gameObject->Renderer.MaterialId = SphereMaterialId_HE;
		}

		if (GDP_IsKeyHeldDown('4')) {
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_HEAT;
			projectileType = ProjectileTypes::HIGH_EXPLOSIVE_ANTI_TANK;
			//m_Bullet.gameObject->Renderer.MaterialId = SphereMaterialId_HEAT;
		}

		if (GDP_IsKeyHeldDown('5')) {
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_HESH;
			projectileType = ProjectileTypes::HIGH_EXPLOSIVES_SQUASH_HEAD;
			//m_Bullet.gameObject->Renderer.MaterialId = SphereMaterialId_HESH;
		}

		if (GDP_IsKeyPressed(32))
		{
			IF_SHOT = true;

			//ShootProjectile(dirToShoot);
		}
	}

	if(IF_SHOT)
		ShootProjectile(dirToShoot);

	// System/Engine update step
	m_PhysicsSystem.UpdateStep(0.01, IF_SHOT);

	/*m_PlayerTank.gameObject->Rotation = glm::angleAxis(AngleBetweenVectors(
		m_PlayerTank.gameObject->Position, m_AimBall.gameObject->Position),
		glm::vec3(0.f, 1.f, 0.f));*/

	// Update the Visual object from the Physics object
	m_AimBall.gameObject->Position = m_AimBall.particle->GetPosition();

	//m_PlayerTank.gameObject->Rotation = glm::quatLookAt(NormalizeVector(m_AimBall.gameObject->Position), glm::vec3(0.f, 1.f, 0.f));

	m_PlayerTank.gameObject->Position = m_PlayerTank.particle->GetPosition();

	m_Bullet.gameObject->Position = m_Bullet.particle->position;

	m_EnemyTank.gameObject->Position = m_EnemyTank.particle->GetPosition();
}

/// <summary>
/// Print provided text to the console for the user
/// </summary>
/// <param name="text"></param>
void ArtilleryGame::DisplayTextToUser(const std::string& text)
{
	std::cout << text << "\n";
}

GameObject* ArtilleryGame::CreateGameObjectByType(const std::string& type)
{
	if (type.compare("Player") == 0) {
		GameObject* go = GDP_CreateGameObject();
		go->Renderer.ShaderId = 1;
		go->Renderer.MeshId = TankModelId;
		go->Renderer.MaterialId = PlayerMaterialId;
		return go;
	}
	if (type.compare("Enemy") == 0) {

		GameObject* go = GDP_CreateGameObject();
		go->Renderer.ShaderId = 1;
		go->Renderer.MeshId = TankModelId;
		go->Renderer.MaterialId = EnemyMaterialId;
		return go;
	}
	if (type.compare("Bullet") == 0) {
		GameObject* go = GDP_CreateGameObject();
		go->Renderer.ShaderId = 1;
		go->Renderer.MeshId = ProjectileModelId;
		go->Renderer.MaterialId = BulletMaterialId;
		go->Position = glm::vec3(0, 0, 0);
		return go;
	}

	// Invalid game object type, return nullptr
	return nullptr;
}