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
#define NONE glm::vec3(0.2, 0.2, 0.2)
#define RED glm::vec3(1, 0, 0)
#define EXPLOSIVES_SIZE 500
//#define DEBUG_LOG_ENABLED
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
unsigned int ExplosiveMaterialId;

unsigned int SphereMaterialId_AP;
unsigned int SphereMaterialId_APCR;
unsigned int SphereMaterialId_HE;
unsigned int SphereMaterialId_HEAT;
unsigned int SphereMaterialId_HESH;
unsigned int SphereMaterialId_NONE;

unsigned int BulletMaterialId_AP;
unsigned int BulletMaterialId_APCR;
unsigned int BulletMaterialId_HE;
unsigned int BulletMaterialId_HEAT;
unsigned int BulletMaterialId_HESH;
unsigned int BulletMaterialId_NONE;

unsigned int SphereModelId;
unsigned int SphereTextureid;
unsigned int bulletTextureId;
unsigned int ExplosiveTextureId;

glm::vec3 dirToShoot;

// FLAGS
bool IF_SHOT = false;
bool IF_HIT = false;
bool IF_MISS = false;
bool EXPLOSION = false;
bool GAME_OVER = false;

ProjectileTypes projectileType;
int AMMO_LOADOUT[5];

// Counters
int SELECTED_PROJECTILE = 0;
int SHOOT_COUNTS = 0;

// flag to be used for counter
int last_shoot_counts = -1;

// Get a random 2D vector (X,Z)
glm::vec3 Random2DVector(int max, int min)
{
	int x = rand() % (max * 2 +1) + min;
	int z = rand() % (max * 2 + 1) + min;
	DEBUG_PRINT("x= %d, z = %d\n", x, z);
	return glm::vec3(x, 10.f, z);
}

// Get a random 3D vector (X, Y, Z)
glm::vec3 Random3DVector(int max, int min)
{
	int x = rand() % (max * 2 +1) + min;
	int y = rand() % (max * 2 +1) + min;
	int z = rand() % (max * 2 + 1) + min;
	return glm::vec3(x / 9.f, y / 9.f, z / 9.f);
}

// Normalize Vector (Not used)
glm::vec3 NormalizeVector(glm::vec3 vec)
{
	float maxValue = 40.f;
	return vec - (-20.f) / maxValue;
}

// Not used, it was an attempt to rotate the tank with the aim ball
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

// Shoot the Projectile towards the direction
void ArtilleryGame::ShootProjectile(glm::vec3 direction)
{
	glm::vec3 target = direction;
	glm::vec3 distance = m_PlayerTank.particle->position - m_Bullet.particle->position;
	float distanceCovered = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
	float totalDistance = sqrt(direction.x * direction.x + direction.y * direction.y + direction.z * direction.z);

	float intTime = 0.f;

	float a_alpha = 0.f;

	// Figure out which projectile to shoot
	switch (projectileType)
	{
	case ProjectileTypes::ARMOR_PIERCING:
		intTime = 0.001;
		a_alpha = 15.f;
		break;
	case ProjectileTypes::ARMOR_PIERCING_COMPOSITE_RIGID:
		intTime = 0.005;
		a_alpha = 15.f;
		break;
	case ProjectileTypes::HIGH_EXPLOSIVES:
		intTime = 0.0009;
		a_alpha = 45.f;
		break;
	case ProjectileTypes::HIGH_EXPLOSIVE_ANTI_TANK:
		intTime = 0.0015;
		a_alpha = 20.f;
		break;
	case ProjectileTypes::HIGH_EXPLOSIVES_SQUASH_HEAD:
		intTime = 0.0007;
		a_alpha = 45.0f;
		break;
	default:
		break;
	}

	float time = 5.f;
	float v_X = totalDistance / time;
	float vel = v_X / cos(a_alpha);
	float v_Y = vel * sin(a_alpha);

	if (distanceCovered <= totalDistance / 2.f - (totalDistance * 0.1))
	{
		if(v_Y < 0)
			target.y -= v_Y * 0.99f;
		else
			target.y += v_Y * 0.99f;
	}
	else if (distanceCovered >  totalDistance / 2.f + (totalDistance * 0.1)) {
		if (v_Y < 0)
			target.y += v_Y;
		else
			target.y -= v_Y;
	}

	m_Bullet.particle->velocity = (target);
	m_Bullet.particle->Integrate(intTime);
}

// Check whether the selected projectile is available
bool canShoot(int i)
{
	if (IF_HIT)
		return false;
	return AMMO_LOADOUT[i] == 1;
}

// Get the name of Projectiles via index
std::string GetProjectileName(int i)
{
	std::string projectileName = "";
	switch (i)
	{
	case 0:
		projectileName = "ARMOR PIERCING";
		break;
	case 1:
		projectileName = "ARMOR PIERCING COMPOSITE RIGID";
		break;
	case 2:
		projectileName = "HIGH EXPLOSIVES";
		break;
	case 3:
		projectileName = "HIGH EXPLOSIVE ANTI TANK";
		break;
	case 4:
		projectileName = "HIGH EXPLOSIVES SQUASH HEAD";
		break;
	default:
		projectileName = "ARMOR PIERCING";
		break;
	}
	return projectileName;
}

// Check whether we hit or miss the enemy
bool ArtilleryGame::HitOrMiss()
{
	if (m_Bullet.particle->GetPosition().y <= 1.1f && m_Bullet.particle->position.x <= m_EnemyTank.particle->position.x + 1 && 
		m_Bullet.particle->position.x >= m_EnemyTank.particle->position.x - 1
		&& m_Bullet.particle->position.z <= m_EnemyTank.particle->position.z + 1
		&& m_Bullet.particle->position.z >= m_EnemyTank.particle->position.z - 1)
	{
		m_Bullet.particle->SetMass(-1.f);
		m_Bullet.particle->SetPosition(m_EnemyTank.particle->position);
		return true;
	}

	if (m_Bullet.particle->GetPosition().y <= 1.1f && m_Bullet.particle->position.x <= m_AimBall.particle->position.x + 1 &&
		m_Bullet.particle->position.x >= m_AimBall.particle->position.x - 1
		&& m_Bullet.particle->position.z <= m_AimBall.particle->position.z + 1
		&& m_Bullet.particle->position.z >= m_AimBall.particle->position.z - 1 && IF_SHOT)
	{
		if(!IF_MISS)
			DisplayTextToUser("ENEMY TANK MISSED :( \t");
		IF_MISS = true;
		IF_SHOT = false;
		m_Bullet.particle->SetPosition(m_PlayerTank.particle->position);
	}
	return false;
}

// Create explosives particle
void ArtilleryGame::CreateExplosives()
{
	for (int i = 0; i < EXPLOSIVES_SIZE; i++)
	{
		GameUnits* gu = new GameUnits();
		glm::vec3 pos = glm::vec3(m_EnemyTank.particle->GetPosition().x, 1.f, m_EnemyTank.particle->GetPosition().z);
		gu->particle = m_PhysicsSystem.CreateParticle(pos);
		gu->particle->velocity = glm::vec3(0.f, 0.f, 0.f);
		m_Explosives.push_back(gu);
	}
}

// Reset the Explosive particles
void ArtilleryGame::ResetExplosives()
{
	if(!m_Explosives.empty())
		for (int i = 0; i < EXPLOSIVES_SIZE; i++)
		{
			m_Explosives[i]->particle->velocity = glm::vec3(0.f, 0.f, 0.f);
			glm::vec3 pos = glm::vec3(m_EnemyTank.particle->GetPosition().x, 1.f, m_EnemyTank.particle->GetPosition().z);
			m_Explosives[i]->particle->SetPosition(pos);
			m_Explosives[i]->gameObject->Position = m_Explosives[i]->particle->position;
		}
}

// Implement explosion effect when enemy is hit
void ArtilleryGame::Explosion(int i)
{
	size_t numParticles = m_Explosives.size();
	int size = 0;
	int max = 0;
	int min = 0;

	switch (i)
	{
	case 0:
		size = numParticles / 5.f;
		max = 3;
		min = -3;
		break;
	case 1:
		size = numParticles / 4.f;
		max = 4;
		min = -4;
		break;
	case 2:
		size = numParticles / 2.f;
		max = 5;
		min = -5;
		break;
	case 3:
		size = numParticles / 1.5f;
		max = 6;
		min = -6;
		break;
	case 4:
		size = numParticles / 1.f;
		max = 7;
		min = -7;
		break;
	default:
		break;
	}

	if (!EXPLOSION)
	{
		EXPLOSION = true;
		for (int i = 0; i < size; i++)
		{
			m_Explosives[i]->particle->velocity = (Random3DVector(max * 10, min * 10));
		}
	}

	for (int i = 0; i < size; i++)
	{
		m_Explosives[i]->particle->ApplyForce(glm::vec3(0, -9.81f, 0));
	}

	for (int i = 0; i < numParticles; i++)
	{
		m_Explosives[i]->particle->Integrate(0.01);
	}

	for (int i = 0; i < numParticles; i++)
	{
		m_Explosives[i]->particle->KillAllForces();
	}
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
	for (int i = 0; i < 5; i++)
		AMMO_LOADOUT[i] = 1;
	// TODO:
	// - Create a Player Tank GameObject
	// - Create an Enemy Tank GameObject
	// - Create projectile(s)
	projectileType = ProjectileTypes::ARMOR_PIERCING;
	m_PlayerTank.particle = m_PhysicsSystem.CreateParticle(Random2DVector(20.f, -20.f));
	m_PlayerTank.gameObject = CreateGameObjectByType("Player");

	m_EnemyTank.particle = m_PhysicsSystem.CreateParticle(Random2DVector(20.f, -20.f));
	m_EnemyTank.gameObject = CreateGameObjectByType("Enemy");

	m_Bullet.particle = m_PhysicsSystem.CreateParticle(m_PlayerTank.particle->GetPosition());
	m_Bullet.gameObject = CreateGameObjectByType("Bullet");
	m_Bullet.gameObject->Scale = glm::vec3(1.5f, 1.5f, 1.5f);
	m_Bullet.particle->isBullet = false;


	GDP_LoadModel(SphereModelId, "assets/models/sphere.obj");
	GDP_LoadTexture(SphereTextureid, "assets/textures/white.png");
	GDP_LoadTexture(bulletTextureId, "assets/textures/white.png");
	GDP_LoadTexture(ExplosiveTextureId, "assets/textures/white.png");

	GDP_CreateMaterial(SphereMaterialId_AP, SphereTextureid, AP);
	GDP_CreateMaterial(SphereMaterialId_APCR, SphereTextureid, APCR);
	GDP_CreateMaterial(SphereMaterialId_HE, SphereTextureid, HE);
	GDP_CreateMaterial(SphereMaterialId_HEAT, SphereTextureid, HEAT);
	GDP_CreateMaterial(SphereMaterialId_HESH, SphereTextureid, HESH);
	GDP_CreateMaterial(SphereMaterialId_NONE, SphereTextureid, NONE);

	GDP_CreateMaterial(BulletMaterialId_AP, bulletTextureId, AP);
	GDP_CreateMaterial(BulletMaterialId_APCR, bulletTextureId, APCR);
	GDP_CreateMaterial(BulletMaterialId_HE, bulletTextureId, HE);
	GDP_CreateMaterial(BulletMaterialId_HEAT, bulletTextureId, HEAT);
	GDP_CreateMaterial(BulletMaterialId_HESH, bulletTextureId, HESH);
	GDP_CreateMaterial(BulletMaterialId_NONE, bulletTextureId, NONE);

	GDP_CreateMaterial(ExplosiveMaterialId, bulletTextureId, RED);

	m_AimBall.particle = m_PhysicsSystem.CreateParticle(glm::vec3(0, 10.f, 0));
	m_AimBall.gameObject = GDP_CreateGameObject();
	m_AimBall.gameObject->Renderer.ShaderId = 1;
	m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_AP;
	m_AimBall.gameObject->Renderer.MeshId = SphereModelId;
	m_AimBall.gameObject->Scale = glm::vec3(0.7, 0.7, 0.7);

	ArtilleryGame::StartNewGame();
	ArtilleryGame::CreateExplosives();
}

/// <summary>
/// Handle everything that needs to be destroyed
/// such as memory de-allocation.
/// </summary>
void ArtilleryGame::Destroy()
{
	DEBUG_PRINT("ArtilleryGame::Destroy\n");
	// TODO:
	for (int i = 0; i < m_Explosives.size(); i++)
		delete m_Explosives[i];
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
	try {
		system("cls");
	}
	catch (int i)
	{

	}
	DisplayTextToUser("Welcome to Artillery!!");
	DisplayTextToUser("Made by Krupesh Parmar (k_parmar180076@fanshaweonline.ca)");
	DisplayTextToUser("Guided by Prof. Lukas Gustafson");

	DisplayTextToUser("\n------------------------------------");
	//DEBUG_PRINT("ArtilleryGame::StartNewGame\n");
	// TODO:
	IF_SHOT = false;
	IF_HIT = false;
	IF_MISS = false;
	GAME_OVER = false;
	EXPLOSION = false;

	m_EnemyTank.particle->SetPosition(Random2DVector(20, -20));
	m_PlayerTank.particle->SetPosition(Random2DVector(20, -20));
	m_AimBall.particle->SetPosition(glm::vec3(0, 10.f, 0));


	m_Bullet.particle->SetMass(1.f);
	m_Bullet.particle->velocity = glm::vec3(0, 0, 0);
	m_Bullet.particle->KillAllForces();
	m_Bullet.particle->SetPosition(m_PlayerTank.particle->GetPosition());
	m_Bullet.particle->isBullet = false;
	SELECTED_PROJECTILE = 0;
	m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_AP;
	m_Bullet.gameObject->Renderer.MaterialId = BulletMaterialId_AP;

	m_PlayerTank.gameObject->Position = m_PlayerTank.particle->GetPosition();

	m_Bullet.gameObject->Position = m_Bullet.particle->GetPosition();

	m_EnemyTank.gameObject->Position = m_EnemyTank.particle->GetPosition();
	ArtilleryGame::ResetExplosives();

	for (int i = 0; i < 5; i++)
		AMMO_LOADOUT[i] = 1;
	SHOOT_COUNTS = 0;
	last_shoot_counts = -1;
	DisplayTextToUser("\n------------------------------------");
	DisplayTextToUser("\nArtillery Challenge!!!");
	DisplayTextToUser("\nYou are green tank and enemy is red tank");
}

/// <summary>
/// Update all bullets here
/// - Call your Physics class to update all projectiles
/// Check for bullet collision (position.y <= 0, hit ground)
/// Check for User input:
/// >>> GDP_IsKeyPressed, GDP_IsKeyHeldDown <-- Case Sensitive
/// >>> example: if (GDP_IsKeyPressed('n') || GDP_IsKeyPressed('N'))	X
/// - Input for changing the bullet direction	(wasd, etc)				X
/// - Input to change the projectile type		(1,2,3,4,5)				X
/// - Input to fire a projectile				(space)					X
/// - Input to start a new game					(n)						X
/// </summary>
void ArtilleryGame::GameUpdate()
{

	// TODO:
	// Start new game
	if (GDP_IsKeyPressed('n'))
	{
		ArtilleryGame::StartNewGame();
	}

	// Update Ammo Loadout
	if (SHOOT_COUNTS != last_shoot_counts)
	{
		last_shoot_counts = SHOOT_COUNTS;
		int shots = 0;
		DisplayTextToUser("\n------------------------------------");
		DisplayTextToUser("Use WASD keys to navigate the aim ball\n");
		DisplayTextToUser("Press the shown numbers to change the Projectiles\n");
		DisplayTextToUser("Press the 'Spacebar' to Shoot!\n");
		DisplayTextToUser("-- Ammo Loadout --");
		for (int i = 0; i < 5; i++)
		{
			if (AMMO_LOADOUT[i] == 1)
			{
				shots++;
				std::cout << i + 1 << " - " << GetProjectileName(i) << "\n";
			}
		}
		if (shots == 0) {
			DisplayTextToUser("Out of Ammo\tPress 'N' to start new game");
			GAME_OVER = true;
		}
	}

	// Check Hit or Miss
	if (HitOrMiss() && !IF_HIT)
	{
		IF_HIT = true;
		DisplayTextToUser("\n------- ENEMY TANK HIT :) \t Press 'N' to start new game! -------");
		GAME_OVER = true;
	}

	// If not shot or game is not over then take user inputs
	if (!IF_SHOT && !GAME_OVER)
	{
		dirToShoot = m_AimBall.particle->GetPosition() - m_Bullet.particle->GetPosition();
		if (GDP_IsKeyHeldDown('a'))
			m_AimBall.particle->ApplyForce(glm::vec3(1, 0, 0));

		if (GDP_IsKeyHeldDown('d'))
			m_AimBall.particle->ApplyForce(glm::vec3(-1, 0, 0));

		if (GDP_IsKeyHeldDown('w'))
			m_AimBall.particle->ApplyForce(glm::vec3(0, 0, 1));

		if (GDP_IsKeyHeldDown('s'))
			m_AimBall.particle->ApplyForce(glm::vec3(0, 0, -1));

		if (GDP_IsKeyHeldDown('1')) {
			SELECTED_PROJECTILE = 0;
			projectileType = ProjectileTypes::ARMOR_PIERCING;
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_AP;
			m_Bullet.gameObject->Renderer.MaterialId = BulletMaterialId_AP;
		}

		if (GDP_IsKeyHeldDown('2')) {
			SELECTED_PROJECTILE = 1;
			projectileType = ProjectileTypes::ARMOR_PIERCING_COMPOSITE_RIGID;
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_APCR;
			m_Bullet.gameObject->Renderer.MaterialId = BulletMaterialId_APCR;
		}

		if (GDP_IsKeyHeldDown('3')) {
			SELECTED_PROJECTILE = 2;
			projectileType = ProjectileTypes::HIGH_EXPLOSIVES;
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_HE;
			m_Bullet.gameObject->Renderer.MaterialId = BulletMaterialId_HE;
		}

		if (GDP_IsKeyHeldDown('4')) {
			SELECTED_PROJECTILE = 3;
			projectileType = ProjectileTypes::HIGH_EXPLOSIVE_ANTI_TANK;
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_HEAT;
			m_Bullet.gameObject->Renderer.MaterialId = BulletMaterialId_HEAT;
		}

		if (GDP_IsKeyHeldDown('5')) {
			SELECTED_PROJECTILE = 4;
			projectileType = ProjectileTypes::HIGH_EXPLOSIVES_SQUASH_HEAD;
			m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_HESH;
			m_Bullet.gameObject->Renderer.MaterialId = BulletMaterialId_HESH;
		}

		if (GDP_IsKeyPressed(32) && canShoot(SELECTED_PROJECTILE))
		{
			m_Bullet.particle->isBullet = true;
			IF_SHOT = true;
			IF_MISS = false;
			SHOOT_COUNTS++;
			AMMO_LOADOUT[SELECTED_PROJECTILE] = 0;
		}
	}
	
	// If Shot but not hit yet, keep the Projectile in motion
	if(IF_SHOT && !IF_HIT)
		ShootProjectile(dirToShoot);

	// Check whether the selected projectile can be shot
	// and update the aimball material accordingly
	if (!canShoot(SELECTED_PROJECTILE))
		m_AimBall.gameObject->Renderer.MaterialId = SphereMaterialId_NONE;

	// System/Engine update step
	m_PhysicsSystem.UpdateStep(0.01);

	// Update the Visual object from the Physics object
	m_AimBall.gameObject->Position = m_AimBall.particle->GetPosition();
	
	m_PlayerTank.gameObject->Position = m_PlayerTank.particle->GetPosition();

	m_Bullet.gameObject->Position = m_Bullet.particle->GetPosition();
	//else m_Bullet.gameObject->Position = m_PlayerTank.particle->GetPosition();

	m_EnemyTank.gameObject->Position = m_EnemyTank.particle->GetPosition();

	// If Hit is detected then simulate explosion effect
	if (IF_HIT)
	{
		for (int i = 0; i < m_Explosives.size(); i++)
		{
			if (m_Explosives[i]->gameObject == nullptr)
			{
				m_Explosives[i]->gameObject = CreateGameObjectByType("Explosives");
				m_Explosives[i]->gameObject->Scale = glm::vec3(0.1f, 0.1f, 0.1f);
			}
			m_Explosives[i]->gameObject->Position = m_Explosives[i]->particle->GetPosition();
		}
		Explosion(SELECTED_PROJECTILE);
	}
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
	if (type.compare("Explosives") == 0) {
		GameObject* go = GDP_CreateGameObject();
		go->Renderer.ShaderId = 1;
		go->Renderer.MeshId = SphereModelId;
		go->Renderer.MaterialId = ExplosiveMaterialId;
		go->Position = m_EnemyTank.particle->GetPosition();
		return go;
	}

	// Invalid game object type, return nullptr
	return nullptr;
}