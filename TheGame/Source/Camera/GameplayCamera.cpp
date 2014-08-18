#include "Camera.h"
#include "GameplayCamera.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// This value needs to be adjusted when you change the bounce. 
// The shake should stop when the values of Viewpoint.x and .y realign with the original x=0.0 and y=10.0
// Otherwise the camera will just realign abruptly
#define CAMERA_SHAKE_TIME 0.333237f

GameplayCamera::GameplayCamera(float distance, Entity* parent) : Camera(parent)
{
	glm::vec3 parentPosition = parent->GetPosition();
	this->distance = distance;
	this->position = glm::vec3(parentPosition.x, parentPosition.y, parentPosition.z - distance);
	this->viewPoint = parentPosition;
	this->upVector = glm::vec3(0, 1, 0);
}

GameplayCamera::~GameplayCamera()
{

}

void GameplayCamera::Update(float dt)
{
	glm::vec3 parentPosition = parent->GetPosition();
	position.z = parentPosition.z - distance;
	viewPoint.z = parentPosition.z;

	if (isShaking && timeElapsed <= CAMERA_SHAKE_TIME)
	{
		// the interval of x should be equal values above and below 0
		// the interval of y should be equal values above and below 10
		// changing the range will change the amplitude of the shake 
		// changing the value of the bounces will change the speed of the shake
		// you have to adjust the interval if you play with the bounce and vice-versa

		if (viewPoint.y <= 9)
		{
			yBounce = .10f;
		}
		else if (viewPoint.y >= 11)
		{
			yBounce = -.10f;
		}

		if (viewPoint.x <= -0.2)
		{
			xBounce = 0.10f;
		}
		else if (viewPoint.x >= 0.2)
		{
			xBounce = -0.10f;
		}

		viewPoint.x += xBounce; 
		viewPoint.y += yBounce;

		timeElapsed += dt;
	}
	else 
	{
		//End shaking
		viewPoint = glm::vec3(0, 10.0f, viewPoint.z);
		timeElapsed = 0.f;
		isShaking = false;
	}
}

glm::vec3 GameplayCamera::GetPosition()
{
	return position;
}

glm::mat4 GameplayCamera::GetViewMatrix() const
{
	return glm::lookAt(position, viewPoint, upVector);
}

void GameplayCamera::Shake()
{
	isShaking = true;
}