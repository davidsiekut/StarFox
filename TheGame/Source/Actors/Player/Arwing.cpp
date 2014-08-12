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
	speedZ = 80.0f;
	movingForward = true;
	barrelRolling = false;
	barrelRollTimer = 0.3f;

	Initialize();
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

	if (barrelRolling)
	{
		float rotationAngle = GetRotationAngle() + dt * rotationSpeed * 40.f;
		rotationAxis = glm::vec3(0.f, 0.f, -1.f);
		SetRotation(rotationAxis, rotationAngle);
		barrelRollTimer -= dt;
	}
	if (barrelRollTimer < 0)
	{
		barrelRollTimer = 0.3f;
		barrelRolling = false;
	}
}

void Arwing::OnCollision(Entity* other)
{
	if (invicibilityFrames <= 0)
	{
		if (other->GetName() == "ENEMY" ||
			other->GetName() == "BUILDING")
		{
			TakeDamage(10);
		}
		else if (other->GetName() == "PEWPEW" && (((PewPew*)other)->owner == "ENEMY"))
		{
			TakeDamage(((PewPew*)other)->damage);
		}

		invicibilityFrames = 2.f;
	}
}

void Arwing::BarrelRollLeft(float dt)
{
	float rotationAngle = GetRotationAngle() + dt * rotationSpeed * 20.f;
	rotationAxis = glm::vec3(0.f, 0.f, -1.f);
	SetRotation(rotationAxis, rotationAngle);
	invicibilityFrames = 0.5f;
}

void Arwing::BarrelRollRight(float dt)
{
	float rotationAngle = GetRotationAngle() + dt * rotationSpeed * 20.f;
	rotationAxis = glm::vec3(0.f, 0.f, 1.f);
	SetRotation(rotationAxis, rotationAngle);
	invicibilityFrames = 0.5f;
}

void Arwing::TiltLeft(float dt)
{
	float rotationAngle = glm::clamp(GetRotationAngle() + dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = glm::vec3(0.f, 0.f, -1.f);
	SetRotation(rotationAxis, rotationAngle);
}

void Arwing::TiltRight(float dt)
{
	float rotationAngle = glm::clamp(GetRotationAngle() + dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = glm::vec3(0.f, 0.f, 1.f);
	SetRotation(rotationAxis, rotationAngle);
}

void Arwing::TiltComplete(float dt)
{
	float rotationAngle = glm::clamp(GetRotationAngle() - dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = GetRotationAxis();
	SetRotation(rotationAxis, rotationAngle);
}


void Arwing::BarrelRollComplete()
{
	SetRotation(rotationAxis, 0.f);
}