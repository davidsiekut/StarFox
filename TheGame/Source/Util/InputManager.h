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
	static bool G_PRESSED;
	static bool L_PRESSED;
	static bool P_PRESSED;
	static bool disabled;
	static void Fire();
	static float fireRepeatRate;

	static Arwing* arwing;

	static float doublePressTimer;
	static std::string lastPressed;
};