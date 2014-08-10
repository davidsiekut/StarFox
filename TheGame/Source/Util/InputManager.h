#pragma once

#include "Scene.h"

class InputManager
{
public:
	static void Initialize(Scene* s, Arwing* a);
	static void Update();

	static float  mouseDeltaX;
	static float  mouseDeltaY;
	static double mouseX;
	static double mouseY;

private:
	static bool shotsFired;
	static void Fire();

	static Scene* scene;
	static Arwing* arwing;
};