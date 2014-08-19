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
// David Siekut

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
