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
		Shoot();
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

	// Update all the pewpews currently in the Scene - Make them travel along the Z axis
	// Until they are too far away for the camera to see in which case they get deleted.
	for (std::vector<Entity*>::iterator it = pewpews.begin(); it < pewpews.end(); ++it)
	{
		// If the pew pew is too far ahead, we delete it.
		if ((*it)->GetPosition().z > position.z + 20.0f)
		{
			//pewpews.pop_back();
		}
		// Update the rest of the pewpews
		(*it)->Update(dt);
	}
}

void Arwing::Shoot()
{
	shotFired = true;

	PewPew* pewpewL = new PewPew(this, glm::vec3(position.x - 0.5f, position.y, position.z));
	PewPew* pewpewR = new PewPew(this, glm::vec3(position.x + 0.5f, position.y, position.z));

	pewpews.push_back(pewpewL);
	pewpews.push_back(pewpewR);

	//TODO create lasers here.
	fprintf(stdout, "pew pew");
}
