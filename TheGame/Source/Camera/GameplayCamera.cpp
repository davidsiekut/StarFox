#include "Camera.h"
#include "GameplayCamera.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define CAMERA_SHAKE_TIME 0.5f

GameplayCamera::GameplayCamera(float distance, Entity* parent) : Camera(parent)
{
	this->distance = distance;

	glm::vec3 parentPosition = parent->GetPosition();
	this->mPosition = glm::vec3(parentPosition.x, parentPosition.y, parentPosition.z - distance);

	mViewPoint = parentPosition;
	mUpVector = glm::vec3(0, 1, 0);
}

GameplayCamera::~GameplayCamera()
{

}

void GameplayCamera::Update(float dt)
{
	glm::vec3 parentPosition = parent->GetPosition();

	mPosition.z = parentPosition.z - distance;
	mViewPoint.z = parentPosition.z;

	//change this value in multiples of 20 for longer shaking
	if (isShaking && timeElapsed <= CAMERA_SHAKE_TIME)
	{
		//the interval of x should be equal values above and below 0
		//the interval of y should be equal values above and below 10
		//changing the range will change the amplitude of the shake 
		//changing the value of the bounces will change the speed of the shake
		if (mViewPoint.y <= 9)
		{
			yBounce = .55f;
		}
		else if (mViewPoint.y >= 11)
		{
			yBounce = -.55f;
		}

		if (mViewPoint.x <= -1.25)
		{
			xBounce = .125f;
		}
		else if (mViewPoint.x >= 1.25)
		{
			xBounce = -.125f;
		}

		mViewPoint.x += xBounce; 
		mViewPoint.y += yBounce;

		timeElapsed += dt;
	}
	else //End shaking
	{
		mViewPoint = glm::vec3(0, 10.0f, mViewPoint.z);
		timeElapsed = 0;
		isShaking = false;
	}
}

glm::mat4 GameplayCamera::GetViewMatrix() const
{
	return glm::lookAt(mPosition, mViewPoint, mUpVector);
}

void GameplayCamera::Shake()
{
	isShaking = true;
}

glm::vec3 GameplayCamera::GetPosition()
{
	return mPosition;
}