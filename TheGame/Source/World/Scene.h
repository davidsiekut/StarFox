#pragma once

#include "Arwing.h"
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

private:
	std::vector<Entity*> entities;
	Camera* camera;
	//glm::vec4 lightPosition;
	//std::vector<Light*> lights;
	Arwing* a;
	EnemyFactory* enemyFactory;

	int lastChunk = 0;
	float enemyTimer = 8.f;
	bool left = false;
};