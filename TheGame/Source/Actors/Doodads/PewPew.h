#pragma once

#include "Entity.h"

class Arwing;

class PewPew : public Entity
{
public:
	PewPew();
	~PewPew();
	void Update(float dt);
	void OnCollision(Entity* other);

	float damage;

protected:

private:
	float timeElapsed = 0.f;
	static const float PEWPEW_SPEED;
	static const float PEWPEW_LIFETIME;
};