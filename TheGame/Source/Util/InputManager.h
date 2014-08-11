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
	static void Disable();

private:
	static bool shotsFired;
	static bool disabled;
	static void Fire();

	static Arwing* arwing;
};