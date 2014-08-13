#include "Camera.h"
#include <Entity.h>
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
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
	return glm::perspective(dtor(45.0f), 4.0f / 3.0f, 0.5f, 800.0f);
}
