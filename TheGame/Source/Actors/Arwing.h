#pragma once

#include "Cube.h"
#include "Entity.h"

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);
	void Update(float dt);

	void SetMovingForwards(bool forward) { movingForward = forward; }

protected:

private:
	void Shoot();

	float rotationAngle;
	float rotationSpeed;
	float speedX;
	float speedY;
	float speedZ;
	bool movingForward;
	bool shotFired;
};