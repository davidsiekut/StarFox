#include "Camera.h"
#include <Entity.h>
#include <GLM/gtx/transform.hpp>

Camera::Camera()
{
	parent = nullptr;
}

Camera::Camera(Entity* parentEntity)
{
	parent = parentEntity;
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewProjectionMatrix() const
{
	// @TODO 1 - Calculate View Projection Matrix

	glm::mat4 viewProjection(GetProjectionMatrix()*GetViewMatrix());

	return viewProjection;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
}
