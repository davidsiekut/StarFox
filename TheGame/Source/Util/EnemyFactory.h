#pragma once

#include "glm/glm.hpp"
#include "Arwing.h"
#include "Scene.h"

class Scene;

class EnemyFactory
{
public:
	enum Direction
	{
		LEFT,
		RIGHT
	};

	EnemyFactory(Arwing* a, Scene* scene);
	~EnemyFactory();

	void SpawnEnemies(int numberEnemies, Direction direction, float y);

	void SpawnCheck(float dt);

private:
	Arwing* a;
	Scene* scene;

	float timer = 3.f;
	bool left = false;
};