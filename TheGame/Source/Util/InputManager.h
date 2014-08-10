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

private:
	static bool shotsFired;
	static void Fire();

	static Arwing* arwing;
};