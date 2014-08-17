#include "Score.h"

Score::Score(glm::vec3 position, glm::vec3 size) : UIElement()
{
	name = "SCORE";
	this->position = position;
	this->size = size;
	SetTextureID(7);
	shaderType = SHADER_TEXTURED;
	Initialize(size);
}

Score::~Score()
{

}

void Score::Update(float dt)
{

}
