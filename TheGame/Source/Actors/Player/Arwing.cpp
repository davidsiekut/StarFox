#pragma once

#include "Arwing.h"
#include "WindowManager.h"
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
}

void Arwing::OnCollision(Entity* other)
{
	if (invicibilityFrames <= 0)
	{
		if (other->GetName() == "ENEMY" ||
			other->GetName() == "CUBE")
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