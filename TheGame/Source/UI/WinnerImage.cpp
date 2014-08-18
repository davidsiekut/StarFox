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