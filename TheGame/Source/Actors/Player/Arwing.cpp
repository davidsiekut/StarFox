#pragma once

#include "Arwing.h"
#include "WindowManager.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>
#include "PewPew.h"
#include "Scene.h"
#include "ThirdPersonCamera.h"

#define BARREL_ROLL_TIME 0.3f

std::vector<Entity::Vertex>* Arwing::bluePrint = new std::vector<Entity::Vertex>();

Arwing::Arwing(Entity *parent) : Entity(parent)
{
	name = "PLAYER";
	size = glm::vec3(2.f, 2.f, 2.f);
	objPath = "../Assets/Models/arwing.obj";

	COLLIDE_X = 7.8f;
	COLLIDE_Y = 1.5f;
	COLLIDE_Z = 2.0f;

	rotationSpeed = 60.0f;
	speedX = 20.0f;
	speedY = 15.0f;
	speedZ = 60.0f;
	movingForward = true;
	isTiltingLeft = false;
	isTiltingRight = false;
	isBarrelRolling = false;
	barrelRollTimer = BARREL_ROLL_TIME;

	bluePrint = Initialize(size, bluePrint);
}

void Arwing::Update(float dt)
{
	if (GetShieldAmount() <= 0)
	{
		movingForward = false;
		Scene::GetInstance().GameOver();
	}

	if (invicibilityFrames > 0)
	{
		invicibilityFrames -= dt;
	}

	if (isBarrelRolling)
	{
		float rotationAngle = GetRotationAngle() + dt * rotationSpeed * 40.f;

		if (isTiltingLeft)
			rotationAxis = glm::vec3(0.f, 0.f, -1.f);
		else
			rotationAxis = glm::vec3(0.f, 0.f, 1.f);

		SetRotation(rotationAxis, rotationAngle);
		barrelRollTimer -= dt;
	}
	if (barrelRollTimer < 0)
	{
		barrelRollTimer = BARREL_ROLL_TIME;
		isBarrelRolling = false;
	}
}

void Arwing::TiltLeft(float dt)
{
	COLLIDE_X = 1.5f;
	COLLIDE_Y = 7.8f;
	COLLIDE_Z = 2.0f;

	float rotationAngle = glm::clamp(GetRotationAngle() + dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = glm::vec3(0.f, 0.f, -1.f);
	SetRotation(rotationAxis, rotationAngle);
}

void Arwing::TiltRight(float dt)
{
	COLLIDE_X = 1.5f;
	COLLIDE_Y = 7.8f;
	COLLIDE_Z = 2.0f;

	float rotationAngle = glm::clamp(GetRotationAngle() + dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = glm::vec3(0.f, 0.f, 1.f);
	SetRotation(rotationAxis, rotationAngle);
}

void Arwing::TiltComplete(float dt)
{
	COLLIDE_X = 7.8f;
	COLLIDE_Y = 1.5f;
	COLLIDE_Z = 2.0f;

	float rotationAngle = glm::clamp(GetRotationAngle() - dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = GetRotationAxis();
	SetRotation(rotationAxis, rotationAngle);
}

void Arwing::OnCollision(Entity* other)
{
	if (invicibilityFrames <= 0 && !isBarrelRolling)
	{
		if (other->GetName() == "ENEMY" ||
			other->GetName() == "BUILDING")
		{
			TakeDamage(10);
			printf("[Player] Shield = %f\n", GetShieldAmount());
		}
		else if (other->GetName() == "PEWPEW" && (((PewPew*)other)->owner == "ENEMY"))
		{
			TakeDamage(((PewPew*)other)->damage);
			printf("[Player] Shield = %f\n", GetShieldAmount());
		}

		invicibilityFrames = 2.f;
	}
}