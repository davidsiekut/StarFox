#include "ThirdPersonCamera.h"
#include "InputManager.h"
#include "WindowManager.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>
#include <algorithm>

ThirdPersonCamera::ThirdPersonCamera(glm::vec3 position, Entity* parentEntity) : Camera(parentEntity)
{
	//Transform variable
	mPosition = position;
	//Set view point at z-axiz
	mViewPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	//Camera mouse and keyboard speeds
	cameraMouseSpeed = 0.075f;
	cameraKeyboardSpeed = 5.5f;
	//Camera mouse angles
	alphaMouseAngle = 0.0f;
	betaMouseAngle = 3.14f;
}

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::Update(float dt)
{
	//get cartesian coordinates from spherical coordinates
	glm::vec3 anglesInCartesianCoordinates(
		cos(alphaMouseAngle) * sin(betaMouseAngle),
		sin(alphaMouseAngle),
		cos(alphaMouseAngle) * cos(betaMouseAngle)
		);

	//get the right vector
	glm::vec3 rightVector = glm::vec3(
		sin(betaMouseAngle - 3.14 / 2.0f), //180 degrees
		0,
		cos(betaMouseAngle - 3.14f / 2.0f)
		);

	//get the up vector
	glm::vec3 upVector = glm::cross(rightVector, anglesInCartesianCoordinates);

	//update view point and upVector
	mViewPoint = mPosition + anglesInCartesianCoordinates;
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
