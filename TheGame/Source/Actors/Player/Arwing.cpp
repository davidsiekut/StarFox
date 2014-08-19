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
// Quang Tran
// Dror Ozgaon
// John Di Girolamo

#pragma once

#include "Arwing.h"
#include "InputManager.h"
#include "PewPew.h"
#include "Scene.h"
#include "ThirdPersonCamera.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#define BARREL_ROLL_TIME 0.3f
#define INV_FRAMES_TIME 0.5f
#define SPEED_X 20.f
#define SPEED_Y 15.f
#define SPEED_Z 60.f
#define SPEED_ROT 60.f

Arwing::Arwing(Entity *parent) : Entity(parent)
{
	this->name = "PLAYER";
	this->size = glm::vec3(2.f, 2.f, 2.f);
	this->objPath = "../Assets/Models/arwing.obj";
	this->shaderType = SHADER_PHONG;
	this->collider = glm::vec3(7.8f, 1.5f, 2.0f);

	this->rotationSpeed = SPEED_ROT;
	this->speedX = SPEED_X;
	this->speedY = SPEED_Y;
	this->speedZ = SPEED_Z;
	this->isMovingForward = true;
	this->isTiltingLeft = false;
	this->isTiltingRight = false;
	this->isBarrelRolling = false;
	this->barrelRollTimer = BARREL_ROLL_TIME;
	this->invicibilityFrames = 0.f;
	this->iddqd = false;
	this->isFlashing = false;

	booster = new ParticleSystem(this, 0.2f, 0.f, SPEED_Z / 1.3f);
	booster->SetPosition(glm::vec3(0.f, -0.25f, -3.f));
	booster->SetMaxParticles(10);
	Scene::GetInstance().AddEntity(booster);
	burnBabyBurn = nullptr;

	Initialize(size);
	Entity::CreateShadow();
}

void Arwing::Update(float dt)
{
	if (GetShieldAmount() <= 0)
	{
		if (burnBabyBurn == nullptr)
		{
			burnBabyBurn = new ParticleSystem(this, 5.f, -1.f, 0.f);
			burnBabyBurn->SetInitialColor(glm::vec3(150.f / 255.f, 75.f / 255.f, 0.f));
			burnBabyBurn->SetParticleSize(5.f);
			burnBabyBurn->SetRedInterPolation([](float red, float dt, float lifetime) -> float { return red; });
			burnBabyBurn->SetGreenInterPolation([](float green, float dt, float lifetime) -> float {
				green += ((75.f / 255.f) / lifetime) * dt;
				return green;
			});
			burnBabyBurn->SetBlueInterPolation([](float blue, float dt, float lifetime) -> float {
				blue += ((150.f / 255.f) / lifetime) * dt;
				return blue;
			});
			burnBabyBurn->SetMainDirection(glm::vec3(0.f, 3.f, 0.f));
			Scene::GetInstance().AddEntity(burnBabyBurn);
			booster->SetMarkedForDeletion();
		}
		Scene::GetInstance().GameOver();
		return;
	}

	if (invicibilityFrames > 0)
	{
		invicibilityFrames -= dt;

		if ((int)(invicibilityFrames * 100) % 5 == 0)
		{
			Flash();
		}
	}
	else
	{
		isFlashing = false;
	}

	if (isBarrelRolling)
	{
		collider.x = 7.8f;
		collider.y = 7.8f;
		collider.z = 2.0f;

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
		collider.x = 7.8f;
		collider.y = 1.5f;
		collider.z = 2.0f;

		barrelRollTimer = BARREL_ROLL_TIME;
		isBarrelRolling = false;
	}
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
		else if (other->GetName() == "PEWPEW" && (((PewPew*)other)->GetOwner() == "ENEMY"))
		{
			resolveHit(((PewPew*)other)->GetDamage());
		}
	}
}

void Arwing::TiltLeft(float dt)
{
	collider.x = 1.5f;
	collider.y = 7.8f;
	collider.z = 2.0f;

	float rotationAngle = glm::clamp(GetRotationAngle() + dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = glm::vec3(0.f, 0.f, -1.f);
	SetRotation(rotationAxis, rotationAngle);
}

void Arwing::TiltRight(float dt)
{
	collider.x = 1.5f;
	collider.y = 7.8f;
	collider.z = 2.0f;

	float rotationAngle = glm::clamp(GetRotationAngle() + dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = glm::vec3(0.f, 0.f, 1.f);
	SetRotation(rotationAxis, rotationAngle);
}

void Arwing::TiltComplete(float dt)
{
	collider.x = 7.8f;
	collider.y = 1.5f;
	collider.z = 2.0f;

	float rotationAngle = glm::clamp(GetRotationAngle() - dt * rotationSpeed * 5.f, 0.f, 90.f);
	rotationAxis = GetRotationAxis();
	SetRotation(rotationAxis, rotationAngle);
}

void Arwing::resolveHit(float damage)
{
	if (!iddqd)
	{
		TakeDamage(damage);
		Scene::GetInstance().GetGPCamera()->Shake();
		invicibilityFrames = INV_FRAMES_TIME;
		//printf("[Player] Shield = %f\n", GetShieldAmount());
	}
}