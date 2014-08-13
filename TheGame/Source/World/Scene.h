#pragma once

#include "Arwing.h"
#include "Camera.h"
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

	Arwing* GetPlayer() { return a; }
	Camera* GetGPCamera() { return camera; }

	void GameOver();
	bool IsGameOver() { return gameOver; }

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

	int lastChunk = 0;
	bool gameOver = false;

	static const unsigned int TERRAIN_PRELOAD;
	static const unsigned int TERRAIN_LOADAHEAD;

	void LoadTextures();
};