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
	static bool L_PRESSED;
	static bool P_PRESSED;
	static bool disabled;
	static bool gottaGoFast;
	static bool youreTooSlow;
	static void Fire();

	static Arwing* arwing;

	static float doublePressTimer;
	static std::string lastPressed;
};