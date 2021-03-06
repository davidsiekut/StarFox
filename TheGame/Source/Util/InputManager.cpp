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
// Dror Ozgaon
// Quang Tran
// David Siekut
// Kwok-Chak Wan
// Vasiliki Boutas
// John Di Girolamo
//

#include "InputManager.h"
#include "PewPew.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define SHIP_CLAMP_X_NEG -10.f
#define SHIP_CLAMP_X_POS 10.f
#define SHIP_CLAMP_Y_NEG 1.5f
#define SHIP_CLAMP_Y_POS 20.f

#define TIMER_BARRELROLL 0.3f

#define FIRE_REPEAT_RATE 0.1f
float InputManager::fireRepeatRate = FIRE_REPEAT_RATE;

double InputManager::mouseX = 0.0f;
double InputManager::mouseY = 0.0f;
float  InputManager::mouseDeltaX = 0.0f;
float  InputManager::mouseDeltaY = 0.0f;

bool InputManager::G_PRESSED = false;
bool InputManager::L_PRESSED = false;
bool InputManager::P_PRESSED = false;
bool InputManager::I_PRESSED = false;
bool InputManager::invertedY = true;
bool InputManager::disabled = false;

bool gottaGoFast = false;
bool youreTooSlow = false;
bool normalSpeed = true;

float boostTimer = 3.0f;

Arwing* InputManager::arwing;

float InputManager::doublePressTimer = TIMER_BARRELROLL;
std::string InputManager::lastPressed = "";

void InputManager::Initialize(Arwing* a)
{
	arwing = a;
}

void InputManager::Update(float dt)
{
	GLFWwindow* w = WindowManager::GetWindow();

	glfwPollEvents();

	glm::vec3 prevRotation = arwing->GetRotationAxis();
	float angle = arwing->GetRotationAngle();

	double x, y;
	glfwGetCursorPos(WindowManager::GetWindow(), &x, &y);
	mouseDeltaX = static_cast<float>(x - mouseX);
	mouseDeltaY = static_cast<float>(y - mouseY);
	mouseX = x;
	mouseY = y;
	//printf("%f, %f\n", mouseDeltaX, mouseDeltaY);

	if (disabled)
	{
		glm::vec3 position = arwing->GetPosition();
		if (position.y > 1.f)
		{
			mouseDeltaX = -0.5f;
			mouseDeltaY = 1.f;
			float rotationAngle = arwing->GetRotationAngle() + dt * 80.f;
			glm::vec3 rotationAxis = glm::vec3(0.f, 1.f, 1.f);
			arwing->SetRotation(rotationAxis, rotationAngle);
			position.y -= dt * 6.f;
			position.z += dt * arwing->speedZ;
			arwing->SetPosition(position);
		}
		else
		{
			//arwing->SetScaling(glm::vec3(0.f, 0.f, 0.f));
		}

		return;
	}

	if (glfwGetKey(w, GLFW_KEY_P) == GLFW_PRESS && !P_PRESSED)
	{
		P_PRESSED = true;
		Renderer::GetInstance().NextShader();
		printf("[Renderer] Current shader: %i\n", Renderer::GetInstance().GetCurrentShader());
	}
	else if (glfwGetKey(w, GLFW_KEY_P) == GLFW_RELEASE)
	{
		P_PRESSED = false;
	}

	if (glfwGetKey(w, GLFW_KEY_L) == GLFW_PRESS && !L_PRESSED)
	{
		L_PRESSED = true;
		printf("[Lighting] Current mode: %i\n", Scene::GetInstance().ChangeLightingMode());
		
	}
	else if (glfwGetKey(w, GLFW_KEY_L) == GLFW_RELEASE)
	{
		L_PRESSED = false;
	}

	if (glfwGetKey(w, GLFW_KEY_I) == GLFW_PRESS && !I_PRESSED)
	{
		I_PRESSED = true;
		invertedY = !invertedY;
		printf("[Controls] Inverted Y: %i\n", invertedY);

	}
	else if (glfwGetKey(w, GLFW_KEY_I) == GLFW_RELEASE)
	{
		I_PRESSED = false;
	}

	if (glfwGetKey(w, GLFW_KEY_G) == GLFW_PRESS && !G_PRESSED)
	{
		G_PRESSED = true;
		Scene::GetInstance().GetPlayer()->iddqd = !Scene::GetInstance().GetPlayer()->iddqd;
		printf("[Player] IDDQD: %d\n", Scene::GetInstance().GetPlayer()->iddqd);
	}
	else if (glfwGetKey(w, GLFW_KEY_G) == GLFW_RELEASE)
	{
		G_PRESSED = false;
	}

	// Shoot action. If the space bar is already pressed then do not create more lasers.
	if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (fireRepeatRate < 0)
		{
			Fire();
			fireRepeatRate = FIRE_REPEAT_RATE;
		}
		else
		{
			fireRepeatRate -= dt;
		}
	}
	else if (glfwGetKey(w, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		fireRepeatRate = 0.0001f;
	}


	// Get the directional input
	glm::vec3 direction = glm::vec3(0, 0, 0);
	if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_UP) == GLFW_PRESS)
	{
		if (invertedY)
		{
			direction.y--;
		}
		else
		{
			direction.y++;
		}
	}
	if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		if (invertedY)
		{
			direction.y++;
		}
		else
		{
			direction.y--;
		}
	}
	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		direction.x--;
	}
	if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		direction.x++;
	}

	//Boost
	if ((glfwGetKey(w, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) && boostTimer == 3.0f)
	{
		gottaGoFast = true;
	}
	if (gottaGoFast == true)
	{
		boostTimer -= dt;
		arwing->speedZ = 110.0f;
	}

	//Brake
	if ((glfwGetKey(w, GLFW_KEY_LEFT_CONTROL) || glfwGetKey(w, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) && boostTimer == 3.0f)
	{
		youreTooSlow = true;
	}
	if (youreTooSlow == true)
	{
		boostTimer -= dt;
		arwing->speedZ = 20.0f;
	}

	//Normal speed
	if (boostTimer <= 0.0f)
	{
		arwing->speedZ = 60.0f;

		if (gottaGoFast == true)
		{
			gottaGoFast = false;
		}
		if (youreTooSlow == true)
		{
			youreTooSlow = false;
		}
	}

	//Refill boostTimer
	if (gottaGoFast == false && youreTooSlow == false)
	{
		boostTimer += dt;
	}

	if (boostTimer >= 3.0f)
	{
		boostTimer = 3.0f;
	}


	glm::vec3 position = arwing->GetPosition();
	// Clamp the position so the ship cannot fly offscreen.
	position.x = glm::clamp(position.x + (direction.x * dt * arwing->speedX), SHIP_CLAMP_X_NEG, SHIP_CLAMP_X_POS);
	position.y = glm::clamp(position.y + (direction.y * dt * arwing->speedY), SHIP_CLAMP_Y_NEG, SHIP_CLAMP_Y_POS);
	if (arwing->isMovingForward) // Constantly move forwards but can be stopped for cinematics or something
	{
		position.z += dt * arwing->speedZ;
	}
	arwing->SetPosition(position);

	/*
		BARREL ROLL CODE BEGIN
	*/

	//sprintf("%s %d %d %f\n", lastPressed.c_str(), isTiltingLeft, isTiltingRight, doublePressTimer);

	// count down the timer for doublepress input
	if (doublePressTimer < TIMER_BARRELROLL)
		doublePressTimer -= dt;

	if (arwing->isBarrelRolling)
		return;

	if (glfwGetKey(w, GLFW_KEY_E) == GLFW_PRESS && !arwing->isTiltingRight)
	{
		if (!arwing->isTiltingLeft)
		{
			if (doublePressTimer != TIMER_BARRELROLL && doublePressTimer > 0 && lastPressed == "E")
			{
				arwing->isTiltingLeft = true;
				arwing->isBarrelRolling = true;
				return;
			}
		}
		lastPressed = "E";
		arwing->isTiltingLeft = true;
		arwing->TiltLeft(dt);

		if (doublePressTimer == TIMER_BARRELROLL)
		{
			doublePressTimer -= 0.0001f;
		}

		return;
	}
	else if (glfwGetKey(w, GLFW_KEY_Q) == GLFW_PRESS && !arwing->isTiltingLeft)
	{
		if (!arwing->isTiltingRight)
		{
			if (doublePressTimer != TIMER_BARRELROLL && doublePressTimer > 0 && lastPressed == "Q")
			{
				arwing->isTiltingRight = true;
				arwing->isBarrelRolling = true;
				return;
			}
		}
		lastPressed = "Q";
		arwing->isTiltingRight = true;
		arwing->TiltRight(dt);

		if (doublePressTimer == TIMER_BARRELROLL)
		{
			doublePressTimer -= 0.0001f;
		}

		return;
	}
	else if (glfwGetKey(w, GLFW_KEY_E) == GLFW_RELEASE || glfwGetKey(w, GLFW_KEY_Q) == GLFW_RELEASE)
	{
		arwing->isTiltingLeft = false;
		arwing->isTiltingRight = false;

		if (doublePressTimer < 0)
		{
			doublePressTimer = TIMER_BARRELROLL;
		}

		arwing->TiltComplete(dt);
	}

	/*
		BARREL ROLL CODE END
	*/
	
	// If there is no direction then align the ship back with the -z axis.
	if (glm::length(direction) > 0)
	{
		// The rotation axis is defined by forwards (z-axis) crossed with the directional input
		glm::vec3 rotationAxis = glm::cross(glm::vec3(0, 0, 1), direction);


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


	if (arwing->isTiltingLeft || arwing->isTiltingRight)
	{
		pewpewL->SetPosition(glm::vec3(arwing->GetPosition().x, arwing->GetPosition().y - 1.f, arwing->GetPosition().z));
		pewpewR->SetPosition(glm::vec3(arwing->GetPosition().x, arwing->GetPosition().y + 1.f, arwing->GetPosition().z));
	}
	else
	{
		pewpewL->SetPosition(glm::vec3(arwing->GetPosition().x + 1.f, arwing->GetPosition().y - 1.f, arwing->GetPosition().z) + 1.f);
		pewpewR->SetPosition(glm::vec3(arwing->GetPosition().x - 3.f, arwing->GetPosition().y - 1.f, arwing->GetPosition().z) + 1.f);
	}

	// Put the pewpews in the list
	Scene::GetInstance().AddEntity(pewpewL);
	Scene::GetInstance().AddEntity(pewpewR);
}

void InputManager::SetDisabled(bool disable)
{
	disabled = disable;
}