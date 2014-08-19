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

#include "HealthBar.h"
#include "Scene.h"
#include "UltraBoss.h"

HealthBar::HealthBar(glm::vec3 position, glm::vec3 size, std::string entityType) : UIElement()
{
	name = "HEALTHBAR";
	this->position = position;
	this->size = size;
	this->entityType = entityType;
	SetTextureID(7);
	shaderType = SHADER_TEXTURED;
	Initialize(size);
}

HealthBar::~HealthBar()
{

}

void HealthBar::Update(float dt)
{
	if (Scene::GetInstance().IsGameWon())
	{
		this->SetScaling(glm::vec3(0.f, 0.f, 0.f));
		this->markedForDeletion = true;
		return;
	}

	if (entityType == "PLAYER")
	{
		float currentShield = Scene::GetInstance().GetPlayer()->GetShieldAmount();
		//printf("%f\n", currentShield);
		this->scaling = glm::vec3(-(currentShield / 100.f), 1.f, 1.f);
		float offset = (2.4f * (100.f - currentShield)) / 2.f;
		this->position = glm::vec3(223 - offset, 30.5, 0.1);
	}
	else if (entityType == "BOSS")
	{
		float currentShield = Scene::GetInstance().GetBoss()->GetShieldAmount();
		//printf("%f\n", currentShield);
		this->scaling = glm::vec3(-(currentShield / 240.f), 1.f, 1.f);
		float offset = ((240.f - currentShield)) / 2.f;
		this->position = glm::vec3(577 + offset, 570.5, 0.1);
	}

	if (-(this->scaling.x) <= 0.25 && this->textureID != 10)
	{
		this->textureID = 10;
	}
	else if (-(this->scaling.x) <= 0.5 && -(this->scaling.x) > 0.25 && this->textureID != 9)
	{
		this->textureID = 9;
	}
}
