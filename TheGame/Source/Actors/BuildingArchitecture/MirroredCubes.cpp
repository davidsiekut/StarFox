#pragma once

#include "Building.h"
#include "MirroredCubes.h"

MirroredCubes::MirroredCubes(Entity* parent, glm::vec2 textureCoordinates) : Entity(parent),
left(this, glm::vec3(BUILDING_SIZE_X / 5.f, BUILDING_SIZE_Y / 3.f, BUILDING_SIZE_Z / 2.f), textureCoordinates),
right(this, glm::vec3(BUILDING_SIZE_X / 5.f, BUILDING_SIZE_Y / 3.f, BUILDING_SIZE_Z / 2.f), textureCoordinates)
{
	name = "CUBE";

	float distance = BUILDING_SIZE_X / 5.f + 1.f;

	left.SetPosition(glm::vec3(-distance, BUILDING_SIZE_Y / 6.f, 0.f));
	right.SetPosition(glm::vec3(distance, BUILDING_SIZE_Y / 6.f, 0.f));
}

MirroredCubes::~MirroredCubes()
{

}

void MirroredCubes::SetShaderType(ShaderType shaderType)
{
	left.SetShaderType(shaderType);
	right.SetShaderType(shaderType);
}

void MirroredCubes::Draw()
{
	left.Draw();
	right.Draw();
}
