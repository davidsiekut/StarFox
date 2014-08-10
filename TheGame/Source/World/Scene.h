#pragma once

#include "Arwing.h"
#include "Chunk.h"
#include "Entity.h"
#include "Camera.h"
#include "EnemyFactory.h"

class Entity;
class EnemyFactory;

class Scene
{
public:
	void Initialize();
	void Update(float dt);
	void Draw();

	void AddEntity(Entity* entity);
	void AddChunk(glm::vec3 pos);

	Arwing* GetPlayer() { return a; }

private:
	std::vector<Entity*> entities;
	std::vector<Chunk*> chunks;
	Camera* camera;
	Arwing* a;
	EnemyFactory* enemyFactory;

	int lastChunk = 0;

	static const unsigned int TERRAIN_PRELOAD;
	static const unsigned int TERRAIN_LOADAHEAD;

	void LoadTextures();

	// move this into a physics class
	bool CheckAABBCollision(Entity* b1, Entity* b2);
};