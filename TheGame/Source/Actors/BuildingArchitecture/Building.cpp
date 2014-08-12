#pragma once

#include "Building.h"
#include "Cube.h"
#include "FilledMirroredCubes.h"

Building::Building(Entity* parent, std::string lSystem) : Entity(parent)
{
	for (unsigned int i = 0; i < lSystem.size(); i++)
	{
		Parse(lSystem.at(i));
	}

	shaderType = SHADER_TEXTURED;
	textureID = 2;

	name = "BUILDING";
	COLLIDE_X = BUILDING_SIZE_X;
	COLLIDE_Y = BUILDING_SIZE_Y;
	COLLIDE_Z = BUILDING_SIZE_Z;
}

void Building::Draw()
{
	for (std::vector<Entity*>::iterator it = blocks.begin(); it < blocks.end(); it++)
	{
		(*it)->Draw();
	}
}

void Building::Parse(char c)
{
	if (c == 'B')
	{
		Cube* c = new Cube(this, glm::vec3(BUILDING_SIZE_X, BUILDING_SIZE_Y, BUILDING_SIZE_Z));
		c->SetPosition(glm::vec3(0.f, BUILDING_SIZE_Y / 2.f, 0.f));
		c->SetShaderType(SHADER_TEXTURED);
		blocks.push_back(c);
	}
	else if (c == 'r')
	{
		for (std::vector<Entity*>::iterator it = blocks.begin(); it < blocks.end(); it++)
		{
			float rotationAngle = (*it)->GetRotationAngle();
			(*it)->SetRotation(glm::vec3(0.f, 1.f, 0.f), rotationAngle + 90.f);
		}
	}
	else if (c == 's')
	{
		for (std::vector<Entity*>::iterator it = blocks.begin(); it < blocks.end(); it++)
		{
			glm::vec3 scaling = (*it)->GetScaling();
			scaling = scaling * 0.5f;
			(*it)->SetScaling(scaling);

			glm::vec3 position = (*it)->GetPosition();
			position.y = -BUILDING_SIZE_Y * scaling.y + BUILDING_SIZE_Y;
			(*it)->SetPosition(position);
		}
	}
	else if (c == 'M')
	{
		FilledMirroredCubes* m = new FilledMirroredCubes(this);
		m->SetShaderType(SHADER_TEXTURED);
		blocks.push_back(m);
	}
}