#pragma once

#include "MirroredCubes.h"

MirroredCubes::MirroredCubes(Entity* parent) : Entity(parent),
left(this, glm::vec3(1.f, 20.f, 5.f)),
right(this, glm::vec3(1.f, 20.f, 5.f))
{
	name = "CUBE";

	float distance = 1.5f;

	left.SetPosition(glm::vec3(-distance, 0.f, 0.f));
	right.SetPosition(glm::vec3(distance, 0.f, 0.f));

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