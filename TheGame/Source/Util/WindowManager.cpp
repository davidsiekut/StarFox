#include "WindowManager.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

GLFWwindow* WindowManager::w = NULL;

void WindowManager::Initialize()
{
	if (!glfwInit())
	{
		fprintf(stderr, "Error in glfwInit()\n");
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2); // major ver
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1); // minor ver
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // core
	glfwWindowHint(GLFW_RESIZABLE, 1);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing

	GLFWmonitor* primary = glfwGetPrimaryMonitor();
	//w = glfwCreateWindow(800, 600, "", primary, NULL); // use this for fullscreen
	w = glfwCreateWindow(800, 600, "", NULL, NULL);

	if (w == NULL)
	{
		fprintf(stderr, "Error in glfwCreateWindow()\n");
		glfwTerminate();
		exit(-1);
	}
}

bool WindowManager::ExitRequested()
{
	return glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(w);
}