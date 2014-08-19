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
//
//

#include "Image.h"
#include "Scene.h"

Image::Image(glm::vec3 position, glm::vec3 size) : UIElement()
{
	name = "Image";
	this->position = position;
	this->size = size;
	SetTextureID(7);
	shaderType = SHADER_TEXTURED;
	Initialize(size);
}

Image::~Image()
{

}

void Image::Update(float dt)
{
	if (Scene::GetInstance().IsGameWon())
	{
		this->SetScaling(glm::vec3(0.f, 0.f, 0.f));
	}
}