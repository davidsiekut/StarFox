#pragma once

#include "glm/glm.hpp"
#include "Arwing.h"
#include "Scene.h"

class Scene;
class UltraBoss;

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
	void SpawnEnemy(int index, Direction direction, float y);
	UltraBoss* SpawnUltraBoss();

private:
	Arwing* a;

	float timer = 4.f;
	bool left = false;

	int enemiesToSpawnTotal = 0;
	int enemiesToSpawnLeft = 0;
};