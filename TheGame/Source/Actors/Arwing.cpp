#pragma once

#include "Arwing.h"
#include "WindowManager.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "WindowManager.h"

// TEMPORARY
#include <time.h>

Arwing::Arwing(Entity *parent) : Entity(parent)
{
	name = "ARWING";
	shotFired = false;
	scaling = glm::vec3(0.02, 0.02, 0.02);

	//shaderType = SHADER_SOLID_COLOR;
	shaderType = SHADER_GOURAUD;

	objPath = "../Assets/Models/arwing.obj";

	Initialize();

	rotationSpeed = 60.0f;
	speed = 18.0f;
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
	position.x = glm::clamp(position.x + (direction.x * dt * speed), -15.f, 15.f);
	position.y = glm::clamp(position.y + (direction.y * dt * speed), -15.f, 15.f);
	if (movingForwards) // Constantly move forwards but can be stopped for cinematics or something
	{
		position.z += dt*speed/4.f;
	}
	SetPosition(position);
}

void Arwing::Shoot()
{
	shotFired = true;

	//TODO create lasers here.
	fprintf(stdout, "pew pew");
}
