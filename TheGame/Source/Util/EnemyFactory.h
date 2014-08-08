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

private:

	Arwing* a;
	Scene* scene;
};