#pragma once

#include "Cube.h"
#include "Entity.h"

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);
	void Update(float dt);
	void Draw();

	void SetMovingForwards(bool forward) { movingForward = forward; }

	std::vector<Entity*> pewpews;

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