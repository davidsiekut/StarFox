#include "Camera.h"
#include <Entity.h>
#include <GLM/gtx/transform.hpp>

Camera::Camera()
{
	parent = NULL;
}

Camera::Camera(Entity* parent)
{
	this->parent = parent;
}

Camera::~Camera()
{

}

glm::mat4 Camera::GetProjectionMatrix() const
{
	return glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
}
