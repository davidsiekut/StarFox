#include "HealthBar.h"

HealthBar::HealthBar(glm::vec3 position, glm::vec3 size) : UIElement()
{
	name = "HEALTHBAR";
	this->position = position;
	this->size = size;
	SetTextureID(1);
	shaderType = SHADER_TEXTURED;
	Initialize(size);
}

HealthBar::~HealthBar()
{

}

void HealthBar::Update(float dt)
{

}
