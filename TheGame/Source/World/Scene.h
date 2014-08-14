#pragma once

#include "Arwing.h"
#include "GameplayCamera.h"
#include "Chunk.h"
#include "EnemyFactory.h"
#include "Entity.h"

class Entity;
class EnemyFactory;

class Scene
{
public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}

	void Initialize();
	void Update(float dt);
	void Draw();

	void AddEntity(Entity* entity);
	void AddChunk(glm::vec3 pos);
	void AddStaticParticleSystem(glm::vec3 pos, float plife, float slife, float speed, float scale);
	void AddStaticParticleSystem(glm::vec3 pos, float plife, float slife, float speed);

	int ChangeLightingMode() { ++lightingMode > 1 ? lightingMode = 0 : lightingMode = 1; return lightingMode; };

	Arwing* GetPlayer() { return a; }
	GameplayCamera* GetGPCamera() { return static_cast<GameplayCamera*>(camera); }

	void GameOver();
	bool IsGameOver() { return gameOver; }

	int score = 0;

private:
	// Hide constructors
	Scene();
	Scene(Scene const&);

	std::vector<Entity*> entities;
	std::vector<Entity*> queued; // queued entities such as pewpews that need to be added
	std::vector<Chunk*> chunks;
	Camera* camera;
	Arwing* a;
	EnemyFactory* enemyFactory;

	int lightingMode = 0; // 0 or 1
	int lastChunk = 0;
	bool gameOver = false;
	bool bossSpawned = false;

	static const unsigned int TERRAIN_PRELOAD;
	static const unsigned int TERRAIN_LOADAHEAD;

	void LoadTextures();
};