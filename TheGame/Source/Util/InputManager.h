#pragma once

#include "Scene.h"

class InputManager
{
public:
	static void Initialize(Arwing* a);
	static void Update(float dt);

	static float  mouseDeltaX;
	static float  mouseDeltaY;
	static double mouseX;
	static double mouseY;
	static void SetDisabled(bool disable);

private:
	static bool shotsFired;
	static bool disabled;
	static void Fire();

	static Arwing* arwing;

	static bool isTiltingLeft;
	static bool isTiltingRight;
	static float doublePressTimer;
	static std::string lastPressed;
};