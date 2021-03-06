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
// Dror Ozgaon
//

#include "ThirdPersonCamera.h"
#include "InputManager.h"
#include "WindowManager.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>

ThirdPersonCamera::ThirdPersonCamera(float radiusLength, Entity* parentEntity) : Camera(parentEntity)
{
	//Camera mouse and keyboard speeds
	cameraMouseSpeed = 0.5f;
	cameraKeyboardSpeed = 5.5f;

	//Camera mouse angles
	alphaMouseAngle = 0.0f;
	betaMouseAngle = -3.14f/2.0f;

	this->radiusLength = radiusLength;
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float dt)
{
	alphaMouseAngle -= InputManager::mouseDeltaY * dt * cameraMouseSpeed;
	betaMouseAngle -= InputManager::mouseDeltaX * dt * cameraMouseSpeed;

	alphaMouseAngle = glm::clamp(alphaMouseAngle, -3.14f/2.f, 3.14f/2.f);

	//get cartesian coordinates from spherical coordinates
	glm::vec3 radius(
		radiusLength * cos(alphaMouseAngle) * cos(betaMouseAngle),
		radiusLength * sin(alphaMouseAngle),
		-radiusLength * cos(alphaMouseAngle) * sin(betaMouseAngle)
		);

	//get the right vector
	glm::vec3 rightVector = glm::cross(radius, glm::vec3(0, 1, 0));

	//get the up vector
	glm::vec3 upVector = glm::cross(rightVector, radius);

	glm::vec3 parentPosition = parent->GetPosition();
	mPosition = parentPosition - radius;

	//update view point and upVector
	mViewPoint = parentPosition;
	mUpVector = upVector;
}

glm::mat4 ThirdPersonCamera::GetViewMatrix() const
{
	// Calculate the View Matrix
	return glm::lookAt(
		mPosition,
		mViewPoint,
		glm::vec3(0.f, 1.f, 0.f)
		);
}

glm::vec3 ThirdPersonCamera::GetPosition()
{
	return mPosition;
}