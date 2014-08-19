//
// COMP 371 Term Project
//
// Created by
// Boutas, Vasiliki   (6220304)
// Di Girolamo, John  (6202918)
// Ozgaon, Dror Asher (6296742)
// Siekut, David      (6329810)
// Tran, Quang        (6339816)
// Wan, Kwok - Chak   (6291643)
//
// Contributions to this file:
// David Siekut
//

#pragma once

#include "Building.h"
#include "Cube.h"
#include "PewPew.h"
#include "FilledMirroredCubes.h"
#include "Scene.h"

Building::Building(Entity* parent, std::string lSystem) : Entity(parent)
{
	for (unsigned int i = 0; i < lSystem.size(); i++)
	{
		Parse(lSystem.at(i));
	}

	name = "BUILDING";
	shaderType = SHADER_PHONG_TEXTURED;
	textureID = 2;

	shield = 1.f;

	collider.x = BUILDING_SIZE_X;
	collider.y = BUILDING_SIZE_Y;
	collider.z = BUILDING_SIZE_Z;

	dustSystem = nullptr;
}

Building::~Building()
{
	if (dustSystem != nullptr)
	{
		dustSystem->SetMarkedForDeletion();
	}
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
		if (dustSystem == nullptr)
		{
			Scene::GetInstance().score += 50;
			dustSystem = new ParticleSystem(this, 1.5f, -1.f, 0.f);
			dustSystem->SetInitialColor(glm::vec3(150.f / 255.f, 75.f / 255.f, 0.f));
			dustSystem->SetParticleSize(7.f);
			dustSystem->SetSpread(6.f);
			dustSystem->SetRedInterPolation([](float red, float dt, float lifetime) -> float { return red; });
			dustSystem->SetGreenInterPolation([](float green, float dt, float lifetime) -> float {
				green += ((75.f/255.f) / lifetime) * dt;
				return green;
			});
			dustSystem->SetBlueInterPolation([](float blue, float dt, float lifetime) -> float {
				blue += (150.f/255.f / lifetime) * dt;
				return blue;
			});
			Scene::GetInstance().AddEntity(dustSystem);
		}

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

void Building::Parse(char c)
{
	if (c == 'B')
	{
		glm::vec3 size = glm::vec3(BUILDING_SIZE_X, BUILDING_SIZE_Y, BUILDING_SIZE_Z);
		glm::vec2 textureCoords = glm::vec2(BUILDING_SIZE_X / 5, BUILDING_SIZE_Y / 5);
		Cube* c = new Cube(this, size, textureCoords);
		c->SetPosition(glm::vec3(0.f, BUILDING_SIZE_Y / 2.f, 0.f));
		c->SetShaderType(SHADER_PHONG_TEXTURED);
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
		glm::vec2 textureCoords = glm::vec2(BUILDING_SIZE_X / 10, BUILDING_SIZE_Y / 10);
		FilledMirroredCubes* m = new FilledMirroredCubes(this, textureCoords);
		m->SetShaderType(SHADER_PHONG_TEXTURED);
		blocks.push_back(m);
	}
}

void Building::OnCollision(Entity* other)
{
	if (other->GetName() == "PEWPEW" && (((PewPew*)other)->GetOwner() == "PLAYER"))
	{
		TakeDamage(((PewPew*)other)->GetDamage());
	}
	if (other->GetName() == "PLAYER")
	{
		TakeDamage(9999);
	}
}