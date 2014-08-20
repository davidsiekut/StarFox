//
// COMP 371 Term Project
//
// Created by
// Boutas, Vasiliki   (6220304)
// Di Girolamo, John  (6202918)
// Ozgaon, Dror Asher (6296742)
// Siekut, David      (6329810)
// Tran, Quang        (6339816)
// Wan, Kwok - Chak   (6291643)
//
// Contributions to this file:
// David Siekut
//

#include "GameTime.h"
#include <GLFW/glfw3.h>

#include <iostream>

double GameTime::frameTimePrev = glfwGetTime();
float  GameTime::frameTime = 0.0f;

void GameTime::Update()
{
	double currentTime = glfwGetTime();
	frameTime = static_cast<float>(currentTime - frameTimePrev);
	frameTimePrev = currentTime;
}