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