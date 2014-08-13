#pragma once

#include "Arwing.h"
#include "WindowManager.h"
#include "InputManager.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "PewPew.h"
#include "Scene.h"
#include "ThirdPersonCamera.h"

#define BARREL_ROLL_TIME 0.3f
#define INV_FRAMES_TIME 0.5f
#define SPEED_Z 60.f

Arwing::Arwing(Entity *parent) : Entity(parent), booster(this, 0.1f, SPEED_Z/1.5f)
{
	name = "PLAYER";
	size = glm::vec3(2.f, 2.f, 2.f);
	objPath = "../Assets/Models/arwing.obj";
	shaderType = SHADER_PHONG;

	COLLIDE_X = 7.8f;
	COLLIDE_Y = 1.5f;
	COLLIDE_Z = 2.0f;

	rotationSpeed = 60.0f;
	speedX = 20.0f;
	speedY = 15.0f;
	speedZ = SPEED_Z;
	movingForward = true;
	isTiltingLeft = false;
	isTiltingRight = false;
	isBarrelRolling = false;
	barrelRollTimer = BARREL_ROLL_TIME;

	Initialize(size);

	booster.SetPosition(glm::vec3(0.f, -0.4f, -3.5f));
	Scene::GetInstance().AddEntity(&booster);
}

void Arwing::Update(float dt)
{
	if (GetShieldAmount() <= 0)
	{
		Scene::GetInstance().GameOver();
		return;
	}

	if (invicibilityFrames > 0)
	{
		invicibilityFrames -= dt;
	}

	if (isBarrelRolling)
	{
		COLLIDE_X = 7.8f;
		COLLIDE_Y = 7.8f;
		COLLIDE_Z = 2.0f;

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
		COLLIDE_X = 7.8f;
		COLLIDE_Y = 1.5f;
		COLLIDE_Z = 2.0f;

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
			resolveHit(5);
		}
		else if (other->GetName() == "PEWPEW" && (((PewPew*)other)->owner == "ENEMY"))
		{
			resolveHit(((PewPew*)other)->damage);
		}
	}
}

void Arwing::resolveHit(float damage)
{
	TakeDamage(damage);
	Scene::GetInstance().GetGPCamera()->Shake();
	invicibilityFrames = INV_FRAMES_TIME;
	printf("[Player] Shield = %f\n", GetShieldAmount());
}