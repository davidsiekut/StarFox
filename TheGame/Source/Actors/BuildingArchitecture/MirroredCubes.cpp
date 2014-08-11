#pragma once

#include "MirroredCubes.h"

MirroredCubes::MirroredCubes(Entity* parent) : Entity(parent),
left(this, glm::vec3(1.f, 5.f, 3.f)),
right(this, glm::vec3(1.f, 5.f, 3.f))
{
	name = "CUBE";

	float distance = 2.f;

	left.SetPosition(glm::vec3(-distance, 2.5f, 0.f));
	right.SetPosition(glm::vec3(distance, 2.5f, 0.f));
}


MirroredCubes::~MirroredCubes()
{

}

void MirroredCubes::Draw()
{
	left.Draw();
	right.Draw();
}
