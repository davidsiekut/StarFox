#pragma once

#include "Entity.h"
#include "ParticleSystem.h"

class UltraBoss : public Entity
{
public:
	UltraBoss(Entity *parent);
	~UltraBoss();
	void Update(float dt);
	void OnCollision(Entity* other);

	float attackCooldown = 10.f;
	static const float UltraBoss::BOSS_ATTACK_CD;

protected:

private:
	static const float UltraBoss::BOSS_SPEED_Z;
};