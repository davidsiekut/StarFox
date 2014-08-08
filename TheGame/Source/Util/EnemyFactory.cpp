#pragma once

#include "EnemyFactory.h"
#include "Enemy.h"

EnemyFactory::EnemyFactory(Arwing* a, Scene* scene) : a(a), scene(scene)
{
}

EnemyFactory::~EnemyFactory()
{
	a = nullptr;
	scene = nullptr;
}

void EnemyFactory::SpawnEnemies(int numberEnemies, Direction direction, float y)
{
	for (int i = 0; i < numberEnemies; i++)
	{
		Enemy* e = new Enemy(NULL, direction, y);
		if (direction == Direction::LEFT)
		{
			e->SetPosition(glm::vec3(20.f + i * 10.f, y, a->GetPosition().z + 50.f));
		}
		else
		{
			e->SetPosition(glm::vec3(-20.f - i * 10.f, y, a->GetPosition().z + 50.f));
		}
		scene->AddEntity(e);
	}
}
