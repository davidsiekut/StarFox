#include "UltraBoss.h"
#include "PewPew.h"
#include "Scene.h"

const float UltraBoss::BOSS_ATTACK_CD = 0.01f;

UltraBoss::UltraBoss(Entity *parent) : Entity(parent)
{
	name = "BOSS"; // ultra boss
	size = glm::vec3(20.f, 20.f, 20.f); // ultra boss, ultra size
	SetRotation(glm::vec3(0.f, 1.f, 0.f), 90.f);
	objPath = "../Assets/Models/sphere.obj"; // ultra sphere
	shaderType = SHADER_PHONG_TEXTURED;
	textureID = 5;
	hasShadow = true;

	this->shield = 240.f; // ultra boss, ultra shield

	// ultra colliders
	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	Initialize(size);
}

UltraBoss::~UltraBoss()
{
	//printf("[Cleanup] Enemy deleted\n");
}

void UltraBoss::Update(float dt)
{
	if (position.y > size.y + 5) // descend from thy heavens, o mighty dolan
		position.y -= dt * 50.f;

	position.z += dt * Scene::GetInstance().GetPlayer()->speedZ;


	if (GetShieldAmount() <= 0)
	{
		glm::vec3 scale = this->GetScaling();

		if (scale.y <= 0.f)
		{
			markedForDeletion = true;
		}

		scale.y -= (dt / 2);
		this->SetScaling(scale);
		Scene::GetInstance().GameWon();
		//markedForDeletion = true;
		return;
	}

	if (attackCooldown <= 0)
	{
		//attackCooldown = 1.f;
		//Scene::GetInstance().Fire();
	}
	else
	{
		attackCooldown -= dt;
	}



}

void UltraBoss::OnCollision(Entity* other)
{
	if (other->GetName() == "PEWPEW" && (((PewPew*)other)->owner == "PLAYER"))
	{
		Scene::GetInstance().score += 5;
		TakeDamage(((PewPew*)other)->damage);
	}
}