#include "Camera.h"
#include "Entity.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <GLM/gtx/transform.hpp>

Camera::Camera()
{
	this->parent = NULL;
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
	return glm::perspective(dtor(CAM_FOV), CAM_ASPECT_X / CAM_ASPECT_Y, CAM_NEAR_CLIP, CAM_FAR_CLIP);
}
