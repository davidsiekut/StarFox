#pragma once

#include "Cube.h"
#include "Entity.h"

class Scene;

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);
	void Update(float dt);

	// Movement constants
	float rotationAngle;
	float rotationSpeed;
	float speedX;
	float speedY;
	float speedZ;
	bool movingForward;
protected:

private:
};