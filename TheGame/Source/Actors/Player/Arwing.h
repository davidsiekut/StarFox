#pragma once

#include "Cube.h"
#include "Entity.h"

class Scene;

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);
	void Update(float dt);
	void OnCollision(Entity* other);

	void SetMovingForwards(bool forward) { movingForward = forward; }

	void TiltLeft(float dt);
	void TiltRight(float dt);
	void TiltComplete(float dt);

	// Movement constants
	float rotationAngle;
	float rotationSpeed;
	float speedX;
	float speedY;
	float speedZ;
	bool movingForward;
	bool isTiltingLeft;
	bool isTiltingRight;
	bool isBarrelRolling;
	float barrelRollTimer;

protected:

private:
	static std::vector<Vertex>* bluePrint;
};