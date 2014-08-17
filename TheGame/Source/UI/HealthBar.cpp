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
	if (entityType == "PLAYER")
	{
		float shield = Scene::GetInstance().GetPlayer()->GetShieldAmount();
		this->scaling = glm::vec3(-(shield / 100.f), 1.f, 1.f);
		
		float currentShield = Scene::GetInstance().GetPlayer()->GetShieldAmount();
		//printf("%f\n", currentShield);
		this->scaling = glm::vec3(-(currentShield / 100.f), 1.f, 1.f);
		float offset = (2.4f * (100.f - currentShield)) / 2.f;
		this->position = glm::vec3(223 - offset, 30.5, 0.1);

		printf("%f\n", this->position.x);
	}
	else if (entityType == "BOSS")
	{
		float shield = Scene::GetInstance().GetBoss()->GetShieldAmount();
		this->scaling = glm::vec3(-(shield / 5000.f), 1.f, 1.f);

		float currentShield = Scene::GetInstance().GetBoss()->GetShieldAmount();
		//printf("%f\n", currentShield);
		this->scaling = glm::vec3(-(currentShield / 5000.f), 1.f, 1.f);
		float offset = (5.7f * (5000.f - currentShield)) / 2.f;
		this->position = glm::vec3(577 + offset, 570.5, 0.1);
	}
}
