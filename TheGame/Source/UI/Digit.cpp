#include "Digit.h"
#include "Scene.h"

Digit::Digit(glm::vec3 position, glm::vec3 size, int placeValue) : UIElement()
{
	name = "DIGIT";
	this->position = position;
	this->size = size;
	this->placeValue = placeValue;
	SetTextureID(17);
	shaderType = SHADER_TEXTURED;
	Initialize(size);
}

Digit::~Digit()
{

}

int Digit::GetDigit(int i)
{
	int score = Scene::GetInstance().score;
	score = score / (glm::pow(10.f, (float)i));
	return score % 10;
}

void Digit::Update(float dt)
{
	SetTextureID(17 + GetDigit(placeValue));
}
