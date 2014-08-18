#pragma once

#include "Arwing.h"
#include "GameplayCamera.h"
#include "Chunk.h"
#include "EnemyFactory.h"
#include "Entity.h"
#include "UIElement.h"

class Entity;
class EnemyFactory;
class UltraBoss;

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
	void RemoveEntity(Entity* entity);
	void AddChunk(glm::vec3 pos);

	int ChangeLightingMode() { ++lightingMode > 1 ? lightingMode = 0 : lightingMode = 1; return lightingMode; };

	Arwing* GetPlayer() { return a; }
	UltraBoss* GetBoss() { return boss; }
	GameplayCamera* GetGPCamera() { return static_cast<GameplayCamera*>(camera); }

	void GameOver();
	bool IsGameOver() { return gameOver; }
	bool IsGameWon() { return gameWon; }
	void GameWon() { gameWon = true; }

	int score = 0;

private:
	// Hide constructors
	Scene();
	Scene(Scene const&);

	std::vector<Entity*> entities;
	std::vector<Entity*> transparentEntities; // Particle systems should be drawn after always.
	std::vector<Entity*> queued; // queued entities such as pewpews that need to be added
	std::vector<Chunk*> chunks;
	std::vector<UIElement*> uiElements;
	Camera* camera;
	Arwing* a;
	EnemyFactory* enemyFactory;
	UltraBoss* boss;

	int lightingMode = 0; // 0 or 1
	int lastChunk = 0;
	bool gameOver = false;
	bool bossSpawned = false;

	static const unsigned int TERRAIN_PRELOAD;
	static const unsigned int TERRAIN_LOADAHEAD;

	void LoadTextures();
	void UpdateEntities(std::vector<Entity*> &entities, float dt);
	void DrawEntities(std::vector<Entity*> &entities);
	void DrawUI(std::vector<UIElement*> &uielem);

	bool gameWon = false;
};