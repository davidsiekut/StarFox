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

	name = "BUILDING";
	COLLIDE_X = 10.f;
	COLLIDE_Y = 15.f;
	COLLIDE_Z = 5.f;
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
		Cube* c = new Cube(this, glm::vec3(5.f, 15.f, 5.f));
		c->SetPosition(glm::vec3(0.f, 7.5f, 0.f));
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
			position.y = -15.f * scaling.y + 15;
			(*it)->SetPosition(position);
		}
	}

	else if (c == 'M')
	{
		blocks.push_back(new FilledMirroredCubes(this));
	}
}