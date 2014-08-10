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

private:
	// Hide constructors
	Scene();
	Scene(Scene& const);

	std::vector<Entity*> entities;
	std::vector<Chunk*> chunks;
	Camera* camera;
	Arwing* a;
	EnemyFactory* enemyFactory;

	int lastChunk = 0;

	static const unsigned int TERRAIN_PRELOAD;
	static const unsigned int TERRAIN_LOADAHEAD;

	void LoadTextures();
};