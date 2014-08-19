//
// COMP 371 Term Project
//
// Created by
// Boutas, Vasiliki   (6220304)
// Di Girolamo, John  (6202918)
// Ozgaon, Dror Asher (6296742)
// Siekut, David      (6329810)
// Tran, Quang        (6339816)
// Wan, Kwok - Chak   (6291643)
//
// Contributions to this file:
// David Siekut
//

#include "UltraBoss.h"
#include "PewPew.h"
#include "Scene.h"

#define BOSS_SCORE_PLUS 1

UltraBoss::UltraBoss(Entity *parent) : Enemy(parent)
{
	this->name = "BOSS"; // ultra boss
	this->size = glm::vec3(20.f, 20.f, 20.f); // ultra size
	this->SetRotation(glm::vec3(0.f, 1.f, 0.f), 90.f); // ultra rotation
	this->objPath = "../Assets/Models/sphere.obj"; // ultra sphere
	this->textureID = 5; // ultra texture
	this->shaderType = SHADER_PHONG_TEXTURED; // ultra shader
	this->collider = glm::vec3(size.x, size.y, size.z); // ultra collider
	this->shield = 1.f; //ultra shield

	Initialize(size);
	Entity::CreateShadow();
}

UltraBoss::~UltraBoss()
{
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteVertexArrays(1, &vertexBufferID);
}

void UltraBoss::Update(float dt)
{
	if (position.y > size.y + 5) // descend from thy heavens, o mighty dolan
		position.y -= dt * 50.f;

	position.z += dt * Scene::GetInstance().GetPlayer()->speedZ;

	if (GetShieldAmount() <= 0)
	{
		glm::vec3 scale = this->GetScaling();

		if (scale.x <= 0.f)
		{
			//this->SetMarkedForDeletion(); // this crashes the game
			Scene::GetInstance().GameWon();
		}
		else
		{
			scale -= (dt / 2);
			this->SetScaling(scale);
		}

		return;
	}

	if (attackCooldown > 0)
	{
		attackCooldown -= dt;
	}
}

void UltraBoss::OnCollision(Entity* other)
{
	if (other->GetName() == "PEWPEW" && (((PewPew*)other)->GetOwner() == "PLAYER"))
	{
		Scene::GetInstance().score += BOSS_SCORE_PLUS;
		TakeDamage(((PewPew*)other)->GetDamage());
	}
}