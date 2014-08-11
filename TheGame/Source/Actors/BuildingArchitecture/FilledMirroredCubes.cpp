#pragma once

#include "FilledMirroredCubes.h"

FilledMirroredCubes::FilledMirroredCubes(Entity* parent) : Entity(parent),
sides(this),
center(this, glm::vec3(6.f, 15.f, 5.f))
{
	center.SetPosition(glm::vec3(0.f, 7.5f, 0.f));
}

void FilledMirroredCubes::Draw()
{
	center.Draw();
	sides.Draw();
}