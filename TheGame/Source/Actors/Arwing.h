#pragma once

#include "Cube.h"
#include "Entity.h"

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);
	void Update(float dt);

	void SetMovingForwards(bool forwards) { movingForwards = forwards; }

protected:

private:

	void Shoot();

	bool shotFired;

	float rotationAngle;
	float rotationSpeed;
	float speed;
	bool movingForwards;
};