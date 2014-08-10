#pragma once

#include "Arwing.h"
#include "Entity.h"

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
	static const float PEWPEW_LIFETIME;
	static const float PEWPEW_SPEED;
};