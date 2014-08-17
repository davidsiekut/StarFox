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
		timer = 5.f;
		enemiesToSpawnTotal = 5;
		enemiesToSpawnLeft = 5;
		left = !left;
	}

	// Only spawn one enemy per frame
	if (enemiesToSpawnLeft > 0)
	{
		if (left)
		{
			SpawnEnemy(enemiesToSpawnTotal - enemiesToSpawnLeft, Direction::LEFT, 12.5f);
		}
		else
		{
			SpawnEnemy(enemiesToSpawnTotal - enemiesToSpawnLeft, Direction::RIGHT, 12.5f);
		}
		enemiesToSpawnLeft--;
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

void EnemyFactory::SpawnEnemy(int index, Direction direction, float y)
{
	Enemy* e = new Enemy(NULL, direction, y, index  * -0.5f);
	if (direction == Direction::LEFT)
	{
		e->SetPosition(glm::vec3(50.f + index * 10.f, y, a->GetPosition().z + 80.f));
	}
	else
	{
		e->SetPosition(glm::vec3(-50.f - index * 10.f, y, a->GetPosition().z + 80.f));
	}
	Scene::GetInstance().AddEntity(e);
}

UltraBoss* EnemyFactory::SpawnUltraBoss()
{
	UltraBoss* u = new UltraBoss(NULL);
	u->SetPosition(glm::vec3(0.f, 500.f, a->GetPosition().z + 100.f));
	Scene::GetInstance().AddEntity(u);

	return u;
}