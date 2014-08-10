#pragma once

#include "Arwing.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>
#include "PewPew.h"
#include "Scene.h"

Arwing::Arwing(Entity *parent) : Entity(parent)
{
	name = "ARWING";
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
	shotFired = false;

	Initialize();
}

void Arwing::Update(float dt)
{
	GLFWwindow* w = WindowManager::GetWindow();

	// Shoot action. If the space bar is already pressed then do not create more lasers.
	if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS && !shotFired)
	{
		AddPewPew();
	}
	else if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		shotFired = false;
	}

	// Get the directional input
	glm::vec3 direction = glm::vec3(0, 0, 0);
	if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
	{
		direction.y++;
	}
	if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
	{
		direction.y--;
	}
	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
	{
		direction.x--;
	}
	if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
	{
		direction.x++;
	}

	// If there is no direction then align the ship back with the -z axis.
	if (glm::length(direction) > 0)
	{
		// The rotation axis is defined by forwards (z-axis) crossed with the directional input
		glm::vec3 rotationAxis = glm::cross(glm::vec3(0, 0, 1), direction);
		glm::vec3 prevRotation = GetRotationAxis();
		float angle = GetRotationAngle();

		if (prevRotation == rotationAxis)
		{
			// Force the rotation to be between 0 and 20 degrees.
			angle = glm::clamp(angle + dt * rotationSpeed, 0.f, 20.f);
			SetRotation(rotationAxis, angle);
		}
		else
		{
			// Reset to the center when changing directions
			SetRotation(rotationAxis, 0);
		}
	}
	else
	{
		float angle = GetRotationAngle();

		// Only decrease the angle up to a certain tolerance.
		if (angle >= 0.5f)
		{
			angle -= rotationSpeed * dt;
			glm::vec3 rotationAxis = GetRotationAxis();
			SetRotation(rotationAxis, angle);
		}
	}

	glm::vec3 position = GetPosition();

	// Clamp the position so the ship cannot fly offscreen.
	position.x = glm::clamp(position.x + (direction.x * dt * speedX), -15.f, 15.f);
	position.y = glm::clamp(position.y + (direction.y * dt * speedY), 1.5f, 25.f);
	if (movingForward) // Constantly move forwards but can be stopped for cinematics or something
	{
		position.z += dt * speedZ;
	}
	SetPosition(position);
}

void Arwing::AddPewPew()
{
	shotFired = true;

	PewPew* pewpewL = new PewPew(NULL, glm::vec3(0.5f , 0.5f, 2.f), this);
	PewPew* pewpewR = new PewPew(NULL, glm::vec3(0.5f, 0.5f, 2.f), this);

	// Set the positions to the current location of the Arwing +- 0.25 so that it shoots from the sides
	pewpewL->SetPosition(glm::vec3(position.x - 1.f, position.y - 0.35f, position.z));
	pewpewR->SetPosition(glm::vec3(position.x + 1.f, position.y - 0.35f, position.z));

	// Put the pewpews in the list
	pewpews.push_back(pewpewL);
	pewpews.push_back(pewpewR);

	//TODO create lasers here.
	fprintf(stdout, "pew pew");
}


void Arwing::Shoot(Scene* scene)
{
	// Add all the pewpews into the scene entities vector and then clear the list (so we don't add the same pewpews over and over)
	for (unsigned int pewpewIndex = 0; pewpewIndex < pewpews.size(); pewpewIndex++)
	{
		scene->AddEntity(pewpews[pewpewIndex]);

		if (pewpewIndex == pewpews.size() - 1)
		{
			pewpews.clear();
		}
	}
}