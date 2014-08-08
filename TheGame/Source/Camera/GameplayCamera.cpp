#include "Camera.h"
#include "GameplayCamera.h"
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
}

glm::mat4 GameplayCamera::GetViewMatrix() const
{
	return glm::lookAt(mPosition, mViewPoint, mUpVector);
}