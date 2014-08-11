#pragma once

#include "MirroredCubes.h"

MirroredCubes::MirroredCubes(Entity* parent) : Entity(parent),
left(this, glm::vec3(1.f, 10.f, 5.f)),
right(this, glm::vec3(1.f, 10.f, 5.f))
{
	name = "CUBE";

	float distance = 2.f;

	left.SetPosition(glm::vec3(-distance, -5.f, 0.f));
	right.SetPosition(glm::vec3(distance, -5.f, 0.f));

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

void MirroredCubes::SetScaleOfCubes(float x, float y)
{
	left.SetScaling(glm::vec3(1.f, y, x));
	right.SetScaling(glm::vec3(1.f, y, x));

	glm::vec3 position = left.GetPosition();
	position.y = -(left.GetSize().y / left.GetScaling().y) / 2.f;
	left.SetPosition(position);

	position.x = -position.x;
	right.SetPosition(position);
}