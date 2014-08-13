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

	EnemyFactory(Arwing* a);
	~EnemyFactory();

	void SpawnCheck(float dt);
	void SpawnEnemies(int numberEnemies, Direction direction, float y);

private:
	Arwing* a;

	float timer = 3.f;
	bool left = false;
};