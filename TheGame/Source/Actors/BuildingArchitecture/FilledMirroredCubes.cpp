#pragma once

#include "Building.h"
#include "FilledMirroredCubes.h"

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

void FilledMirroredCubes::SetShaderType(ShaderType shaderType)
{
	this->shaderType = shaderType;
	center.SetShaderType(shaderType);
	sides.SetShaderType(shaderType);
}