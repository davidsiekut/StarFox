#pragma once

#include "FilledMirroredCubes.h"

FilledMirroredCubes::FilledMirroredCubes(Entity* parent) : Entity(parent),
sides(this),
center(this, glm::vec3(3.f, 7.5f, 5.f))
{
	center.SetPosition(glm::vec3(0.f, 3.75f, 0.f));
}

void FilledMirroredCubes::Draw()
{
	center.Draw();
	sides.Draw();
}