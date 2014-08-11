#pragma once

#include "MirroredCubes.h"

MirroredCubes::MirroredCubes(Entity* parent) : Entity(parent),
left(this, glm::vec3(2.f, 10.f, 3.f)),
right(this, glm::vec3(2.f, 10.f, 3.f))
{
	name = "CUBE";

	float distance = 4.f;

	left.SetPosition(glm::vec3(-distance, 5.f, 0.f));
	right.SetPosition(glm::vec3(distance, 5.f, 0.f));

	COLLIDE_X = left.COLLIDE_X + right.COLLIDE_Y + distance + left.COLLIDE_X / 2;
	COLLIDE_Y = left.COLLIDE_Y;
	COLLIDE_Z = left.COLLIDE_Z;
}


MirroredCubes::~MirroredCubes()
{

}

void MirroredCubes::Draw()
{
	left.Draw();
	right.Draw();
}
