#include "InputManager.h"
#include "PewPew.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define SHIP_CLAMP_X_NEG -10.f
#define SHIP_CLAMP_X_POS 10.f
#define SHIP_CLAMP_Y_NEG 1.5f
#define SHIP_CLAMP_Y_POS 20.f

#define TIMER_BARRELROLL 0.2f

double InputManager::mouseX = 0.0f;
double InputManager::mouseY = 0.0f;
float  InputManager::mouseDeltaX = 0.0f;
float  InputManager::mouseDeltaY = 0.0f;

bool InputManager::shotsFired = false;
bool InputManager::disabled = false;
Arwing* InputManager::arwing;

float InputManager::doublePressTimer = TIMER_BARRELROLL;
bool InputManager::isTiltingLeft = false;
bool InputManager::isTiltingRight = false;
std::string InputManager::lastPressed = "";

void InputManager::Initialize(Arwing* a)
{
	arwing = a;
}

void InputManager::Update(float dt)
{
	GLFWwindow* w = WindowManager::GetWindow();

	glfwPollEvents();

	double x, y;
	glfwGetCursorPos(WindowManager::GetWindow(), &x, &y);
	mouseDeltaX = static_cast<float>(x - mouseX);
	mouseDeltaY = static_cast<float>(y - mouseY);
	mouseX = x;
	mouseY = y;

	if (disabled)
	{
		return;
	}

	// Shoot action. If the space bar is already pressed then do not create more lasers.
	if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS && !shotsFired)
	{
		shotsFired = true;
		Fire();
	}
	else if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		shotsFired = false;
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

	glm::vec3 position = arwing->GetPosition();
	// Clamp the position so the ship cannot fly offscreen.
	position.x = glm::clamp(position.x + (direction.x * dt * arwing->speedX), SHIP_CLAMP_X_NEG, SHIP_CLAMP_X_POS);
	position.y = glm::clamp(position.y + (direction.y * dt * arwing->speedY), SHIP_CLAMP_Y_NEG, SHIP_CLAMP_Y_POS);
	if (arwing->movingForward) // Constantly move forwards but can be stopped for cinematics or something
	{
		position.z += dt * arwing->speedZ;
	}
	arwing->SetPosition(position);

	//printf("%d %d %f\n", isTiltingLeft, isTiltingRight, doublePressTimer);

	if (doublePressTimer < TIMER_BARRELROLL)
		doublePressTimer -= dt;

	if (arwing->barrelRolling)
		return;

	if (glfwGetKey(w, GLFW_KEY_Q) == GLFW_PRESS)
	{
		lastPressed = "Q";
		if (!isTiltingLeft)
		{
			if (doublePressTimer != TIMER_BARRELROLL && doublePressTimer > 0 && lastPressed == "Q")
			{
				arwing->barrelRolling = true;
				return;
			}
		}

		if (doublePressTimer == TIMER_BARRELROLL)
		{
			doublePressTimer -= 0.0001f;
		}

		isTiltingLeft = true;
		arwing->TiltLeft(dt);

		return;
	}
	else if (glfwGetKey(w, GLFW_KEY_E) == GLFW_PRESS)
	{
		lastPressed = "E";
		if (!isTiltingRight)
		{
			if (doublePressTimer != TIMER_BARRELROLL && doublePressTimer > 0 && lastPressed == "E")
			{
				arwing->barrelRolling = true;
				return;
			}
		}

		if (doublePressTimer == TIMER_BARRELROLL)
		{
			doublePressTimer -= 0.0001f;
		}

		isTiltingRight = true;
		arwing->TiltRight(dt);

		return;
	}
	else if (glfwGetKey(w, GLFW_KEY_E) == GLFW_RELEASE || glfwGetKey(w, GLFW_KEY_Q) == GLFW_RELEASE)
	{
		isTiltingLeft = isTiltingRight = false;
		if (doublePressTimer < 0)
		{
			doublePressTimer = TIMER_BARRELROLL;
		}

		arwing->TiltComplete(dt);
	}

	// If there is no direction then align the ship back with the -z axis.
	if (glm::length(direction) > 0)
	{
		// The rotation axis is defined by forwards (z-axis) crossed with the directional input
		glm::vec3 rotationAxis = glm::cross(glm::vec3(0, 0, 1), direction);
		glm::vec3 prevRotation = arwing->GetRotationAxis();
		float angle = arwing->GetRotationAngle();

		if (prevRotation == rotationAxis)
		{
			// Force the rotation to be between 0 and 20 degrees.
			angle = glm::clamp(angle + dt * arwing->rotationSpeed, 0.f, 20.f);
			arwing->SetRotation(rotationAxis, angle);
		}
		else
		{
			// Reset to the center when changing directions
			arwing->SetRotation(rotationAxis, 0);
		}
	}
	else
	{
		float angle = arwing->GetRotationAngle();

		// Only decrease the angle up to a certain tolerance.
		if (angle >= 0.5f)
		{
			angle -= arwing->rotationSpeed * dt;
			glm::vec3 rotationAxis = arwing->GetRotationAxis();
			arwing->SetRotation(rotationAxis, angle);
		}
	}
}

void InputManager::Fire()
{
	PewPew* pewpewL = new PewPew("PLAYER");
	PewPew* pewpewR = new PewPew("PLAYER");

	// TODO rotate pewpews based on player rotation
	// Set the positions to the current location of the Arwing +- 0.25 so that it shoots from the sides
	pewpewL->SetPosition(glm::vec3(arwing->GetPosition().x - 1.f, arwing->GetPosition().y - 0.35f, arwing->GetPosition().z));
	pewpewR->SetPosition(glm::vec3(arwing->GetPosition().x + 1.f, arwing->GetPosition().y - 0.35f, arwing->GetPosition().z));

	pewpewL->SetRotation(arwing->GetRotationAxis(), arwing->GetRotationAngle());
	pewpewR->SetRotation(arwing->GetRotationAxis(), arwing->GetRotationAngle());

	// Put the pewpews in the list
	Scene::GetInstance().AddEntity(pewpewL);
	Scene::GetInstance().AddEntity(pewpewR);
}

void InputManager::SetDisabled(bool disable)
{
	disabled = disable;
}