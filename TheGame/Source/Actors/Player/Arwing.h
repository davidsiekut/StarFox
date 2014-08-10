#pragma once

#include "Cube.h"
#include "Entity.h"

class Scene;

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);
	void Update(float dt);
	void SetMovingForwards(bool forward) { movingForward = forward; }
	void Shoot(Scene* scene);

	// Movement constanst
	float rotationAngle;
	float rotationSpeed;
	float speedX;
	float speedY;
	float speedZ;
	bool movingForward;
protected:

private:
};