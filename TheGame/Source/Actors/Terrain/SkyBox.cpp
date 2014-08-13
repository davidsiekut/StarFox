#include "SkyBox.h"

const float SkyBox::SKYBOX_X = 1024.f;
const float SkyBox::SKYBOX_Y = 512.f;

SkyBox::SkyBox(Entity *parent, Arwing *arwing) : Cube(parent, glm::vec3(SKYBOX_X, SKYBOX_Y, 0.1f), glm::vec2(SKYBOX_X / 1024.f, SKYBOX_Y / 512.f))
{
	name = "SKYBOX";
	size = glm::vec3(SKYBOX_X, SKYBOX_Y, 1.f);
	this->arwing = arwing;
	//this->SetPosition(glm::vec3(parent->GetPosition().x, parent->GetPosition().y, parent->GetPosition().z + 100));
	objPath = "../Assets/Models/cube.obj";
	shaderType = SHADER_TEXTURED;
	textureID = 4;

	Initialize(size);
}

SkyBox::~SkyBox()
{
	//printf("[Cleanup] Chunk deleted\n");
}

void SkyBox::Update(float dt)
{
	position.y = 200;
	position.z = arwing->GetPosition().z + 600;
}