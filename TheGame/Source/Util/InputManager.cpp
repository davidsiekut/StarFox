#include "InputManager.h"
#include "WindowManager.h"
#include <GLFW/glfw3.h>

double InputManager::mouseX = 0.0f;
double InputManager::mouseY = 0.0f;
float  InputManager::mouseDeltaX = 0.0f;
float  InputManager::mouseDeltaY = 0.0f;

void InputManager::Update()
{
	glfwPollEvents();

	double x, y;
	glfwGetCursorPos(WindowManager::GetWindow(), &x, &y);
	mouseDeltaX = static_cast<float>(x - mouseX);
	mouseDeltaY = static_cast<float>(y - mouseY);
	mouseX = x;
	mouseY = y;
}