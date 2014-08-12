#pragma once

#include "Building.h"
#include "Cube.h"
#include "PewPew.h"
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

	GetShieldAmount();
}

void Building::Draw()
{
	for (std::vector<Entity*>::iterator it = blocks.begin(); it < blocks.end(); it++)
	{
		(*it)->Draw();
	}
}

void Building::Update(float dt)
{
	if (GetShieldAmount() < 0)
	{
		timeElapsed += dt;
		glm::vec3 position = GetPosition();

		position.x = verticalAxis + glm::sin(timeElapsed * BUILDING_SHAKE_SPEED) * BUILDING_SHAKE_AMPLITUDE;

		position.y -= BUILDING_SINK_SPEED * dt;
		SetPosition(position);
	}
}

void Building::SetPosition(glm::vec3 position)
{
	Entity::SetPosition(position);
	verticalAxis = position.x;
}

void Building::OnCollision(Entity* other)
{
	if (other->GetName() == "PEWPEW" && (((PewPew*)other)->owner == "PLAYER"))
	{
		TakeDamage(((PewPew*)other)->damage);
	}
}

void Building::Parse(char c)
{
	if (c == 'B')
	{
		glm::vec3 size = glm::vec3(BUILDING_SIZE_X, BUILDING_SIZE_Y, BUILDING_SIZE_Z);
		glm::vec2 textureCoords = glm::vec2(BUILDING_SIZE_X / 15, BUILDING_SIZE_Y / 15);
		Cube* c = new Cube(this, size, textureCoords);
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
		glm::vec2 textureCoords = glm::vec2(BUILDING_SIZE_X / 15, BUILDING_SIZE_Y / 15);
		FilledMirroredCubes* m = new FilledMirroredCubes(this, textureCoords);
		m->SetShaderType(SHADER_TEXTURED);
		blocks.push_back(m);
	}
}