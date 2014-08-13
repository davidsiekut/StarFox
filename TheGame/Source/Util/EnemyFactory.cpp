#pragma once

#include "EnemyFactory.h"
#include "Enemy.h"
#include "UltraBoss.h"

EnemyFactory::EnemyFactory(Arwing* a) : a(a)
{
}

EnemyFactory::~EnemyFactory()
{
	a = nullptr;
}

void EnemyFactory::SpawnCheck(float dt)
{
	if (Scene::GetInstance().IsGameOver())
	{
		return;
	}

	timer -= dt;
	if (timer < 0)
	{
		timer = 3.f;
		if (left)
		{
			SpawnEnemies(5, EnemyFactory::Direction::LEFT, 12.5f);
			
			left = !left;
		}
		else
		{
			SpawnEnemies(5, EnemyFactory::Direction::RIGHT, 12.5f);
			left = !left;
		}
	}
}

void EnemyFactory::SpawnEnemies(int numberEnemies, Direction direction, float y)
{
	for (int i = 0; i < numberEnemies; i++)
	{
		Enemy* e = new Enemy(NULL, direction, y, i * -0.5f);
		if (direction == Direction::LEFT)
		{
			e->SetPosition(glm::vec3(50.f + i * 10.f, y, a->GetPosition().z + 80.f));
		}
		else
		{
			e->SetPosition(glm::vec3(-50.f - i * 10.f, y, a->GetPosition().z + 80.f));
		}
		Scene::GetInstance().AddEntity(e);
	}
}

void EnemyFactory::SpawnUltraBoss()
{
	UltraBoss* u = new UltraBoss(NULL);
	u->SetPosition(glm::vec3(0.f, 500.f, a->GetPosition().z + 100.f));
	Scene::GetInstance().AddEntity(u);
}