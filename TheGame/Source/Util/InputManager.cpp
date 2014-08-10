#include "InputManager.h"
#include "PewPew.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>

double InputManager::mouseX = 0.0f;
double InputManager::mouseY = 0.0f;
float  InputManager::mouseDeltaX = 0.0f;
float  InputManager::mouseDeltaY = 0.0f;

bool InputManager::shotsFired = false;
Scene* InputManager::scene;
Arwing* InputManager::arwing;

void InputManager::Initialize(Scene* s, Arwing* a)
{
	scene = s;
	arwing = a;
}

void InputManager::Update()
{
	GLFWwindow* w = WindowManager::GetWindow();

	glfwPollEvents();

	double x, y;
	glfwGetCursorPos(WindowManager::GetWindow(), &x, &y);
	mouseDeltaX = static_cast<float>(x - mouseX);
	mouseDeltaY = static_cast<float>(y - mouseY);
	mouseX = x;
	mouseY = y;

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
}

void InputManager::Fire()
{
	PewPew* pewpewL = new PewPew();
	PewPew* pewpewR = new PewPew();

	// Set the positions to the current location of the Arwing +- 0.25 so that it shoots from the sides
	pewpewL->SetPosition(glm::vec3(arwing->GetPosition().x - 1.f, arwing->GetPosition().y - 0.35f, arwing->GetPosition().z));
	pewpewR->SetPosition(glm::vec3(arwing->GetPosition().x + 1.f, arwing->GetPosition().y - 0.35f, arwing->GetPosition().z));

	// Put the pewpews in the list
	scene->AddEntity(pewpewL);
	scene->AddEntity(pewpewR);
}