#pragma once

#include "Entity.h"

class Arwing : public Entity
{
public:
	Arwing(Entity *parent);

	void Update(float dt);

protected:

private:
	void StrafeUp(float dt);
	void StrafeUpRelease(float dt);
	void StrafeDown(float dt);
	void StrafeDownRelease(float dt);

	float rotationAngle;
	glm::vec3 position;
	float speed;
};