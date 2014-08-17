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

	this->shield = 5000.f; // ultra boss, ultra shield

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
	if (GetShieldAmount() <= 0)
	{
		markedForDeletion = true;
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

	if (position.y > size.y + 5) // descend from thy heavens, o mighty dolan
		position.y -= dt * 50.f;

	position.z += dt * Scene::GetInstance().GetPlayer()->speedZ;

}

void UltraBoss::OnCollision(Entity* other)
{
	if (other->GetName() == "PEWPEW" && (((PewPew*)other)->owner == "PLAYER"))
	{
		TakeDamage(((PewPew*)other)->damage);
	}
}