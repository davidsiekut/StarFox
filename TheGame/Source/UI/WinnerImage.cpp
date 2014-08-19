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
// David Siekut
//

#include "WinnerImage.h"

WinnerImage::WinnerImage(glm::vec3 position, glm::vec3 size) : UIElement()
{
	name = "Image";
	this->position = position;
	this->size = size;
	SetTextureID(27);
	shaderType = SHADER_TEXTURED;
	this->SetScaling(glm::vec3(0.f));

	Initialize(size);
}

WinnerImage::~WinnerImage()
{

}

void WinnerImage::Update(float dt)
{
	glm::vec3 scale = this->GetScaling();
	if (scale.x > -1)
	{
		scale -= (dt / 2);
		this->SetScaling(scale);
	}
}