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
	if (Scene::GetInstance().GetBoss() == NULL)
	{
		this->markedForDeletion = true;
	}
}

