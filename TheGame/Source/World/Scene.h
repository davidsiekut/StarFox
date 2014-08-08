#pragma once

#include "Arwing.h"
#include "Chunk.h"
#include "Entity.h"

class Entity;

class Scene
{
public:
	void Initialize();
	void Update(float dt);
	void Draw();

	void AddEntity(Entity* entity);
	void AddChunk(glm::vec3 pos);

private:
	std::vector<Entity*> entities;
	std::vector<Chunk*> chunks;

	Arwing* a;

	int lastChunk = 0;

	static const unsigned int TERRAIN_PRELOAD;
	static const unsigned int TERRAIN_LOADAHEAD;
};