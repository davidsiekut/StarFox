#pragma once

#include "FilledMirroredCubes.h"

#define BUILDING_SIZE_X 10.f
#define BUILDING_SIZE_Y 30.f
#define BUILDING_SIZE_Z 10.f

FilledMirroredCubes::FilledMirroredCubes(Entity* parent) : Entity(parent),
sides(this),
center(this, glm::vec3(BUILDING_SIZE_X / 2.f, BUILDING_SIZE_Y / 2.f, BUILDING_SIZE_Z))
{
	center.SetPosition(glm::vec3(0.f, BUILDING_SIZE_Y / 4.f, 0.f));
}

void FilledMirroredCubes::Draw()
{
	center.Draw();
	sides.Draw();
}