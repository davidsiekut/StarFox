#pragma once

class InputManager
{
public:
	static void Update();
	static float  mouseDeltaX;
	static float  mouseDeltaY;

private:
	static double mouseX;
	static double mouseY;
};