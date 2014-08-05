#pragma once

class InputManager
{
public:
	static void Update();

private:
	static double mouseX;
	static double mouseY;
	static float  mouseDeltaX;
	static float  mouseDeltaY;
};