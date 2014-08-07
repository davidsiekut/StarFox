#include "GameTime.h"
#include <GLFW/glfw3.h>

#include <iostream>

double GameTime::frameTimePrev = glfwGetTime();
float  GameTime::frameTime = 0.0f;

void GameTime::Update()
{
	double currentTime = glfwGetTime();
	frameTime = static_cast<float>(currentTime - frameTimePrev);
	frameTimePrev = currentTime;
}