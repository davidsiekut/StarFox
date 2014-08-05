#pragma once

struct GLFWwindow;

class WindowManager
{
public:
	static void Initialize();
	static bool ExitRequested();
	static GLFWwindow* GetWindow() { return w; }

private:
	static GLFWwindow* w;
};