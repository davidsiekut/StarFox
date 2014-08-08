#include "Arwing.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "WindowManager.h"

// TEMPORARY
#include <time.h>

#include <iostream>

using namespace std;

Arwing::Arwing(Entity *parent) : Entity(parent)
{
	name = "ARWING";
	scaling = glm::vec3(0.02, 0.02, 0.02);

	shaderType = SHADER_SOLID_COLOR;
	//shaderType = SHADER_GOURAUD;

	objPath = "../Assets/Models/arwing.obj";

	Initialize();

	rotationAngle = GetRotationAngle();
	position = GetPosition();
	speed = 50.0f;
}

void Arwing::Update(float dt)
{
	if (glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		StrafeUp(dt);
		if (glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_UP) == GLFW_RELEASE || glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_W) == GLFW_RELEASE)
		{
			StrafeUpRelease(dt);
		}
	}
	else if (glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		StrafeDown(dt);
		if (glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_DOWN) == GLFW_RELEASE || glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_S) == GLFW_RELEASE)
		{
			StrafeDownRelease(dt);
		}
	}
	else if (glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS)
	{

	}
	else if (glfwGetKey(WindowManager::GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS)
	{

	}
}

void Arwing::StrafeUp(float deltaTime)
{
	glm::vec3 rotationAxis(1.0, 0.0, 0.0);

	if (rotationAngle > -45.0f)
	{
		rotationAngle -= 5.0f;
		SetRotation(rotationAxis, rotationAngle);
		cout << rotationAngle << endl ;
	}

	if (position.y < 6.0f)
	{
		position.y += 1.0f * deltaTime * speed;
		SetPosition(glm::vec3(position.x, position.y, position.z));
	}
}

void Arwing::StrafeUpRelease(float deltaTime)
{
	glm::vec3 rotationAxis(1.0, 0.0, 0.0);

	rotationAngle = 0.0f;
	SetRotation(rotationAxis, rotationAngle);

}

void Arwing::StrafeDown(float deltaTime)
{
	glm::vec3 rotationAxis(1.0, 0.0, 0.0);

	if (rotationAngle < -30.0f)
	{
		rotationAngle -= 5.0f;
		SetRotation(rotationAxis, rotationAngle);
	}

	if (position.y > -5.0f)
	{
		position.y -= 1.0f * deltaTime * speed;
		SetPosition(glm::vec3(position.x, position.y, position.z));
	}
}

void Arwing::StrafeDownRelease(float deltaTime)
{
	glm::vec3 rotationAxis(1.0, 0.0, 0.0);

	rotationAngle = 0.0f;
	SetRotation(rotationAxis, rotationAngle);
}

