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
// Vasiliki Boutas
//

#include "UIElement.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>

UIElement::UIElement() : Entity(NULL)
{
	name = "UIELEMENT";
	SetScaling(glm::vec3(-1.f, 1.f, 1.f));
	objPath = "../Assets/Models/cube.obj";
	shaderType = SHADER_TEXTURED;
}

UIElement::~UIElement()
{
	                   
}

glm::mat4 UIElement::GetWorldMatrix() const
{
	glm::mat4 W(1.0f);

	W = glm::translate(W, position);
	W = glm::rotate(W, dtor(rotationAngle), rotationAxis);
	W = glm::scale(W, scaling);

	return W;
}
