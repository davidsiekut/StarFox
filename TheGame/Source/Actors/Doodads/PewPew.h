#pragma once

#include "Arwing.h"
#include "Entity.h"
#include "Bloom.h"

class PewPew : public Entity
{
public:
	PewPew(std::string owner);
	PewPew(std::string owner, glm::vec3 direction);
	~PewPew();

	void Update(float dt);
	void OnCollision(Entity* other);

	float damage;
	std::string owner;

	static const float PEWPEW_LIFETIME;
	static const float PEWPEW_SPEED_PLAYER;
	static const float PEWPEW_SPEED_ENEMY;
	static const float BLOOM_SCALE;

protected:

private:
	float timeElapsed = 0.f;
	glm::vec3 direction;

	Bloom* bloom;

	void Init();
};