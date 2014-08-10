#include "PewPew.h"

const float PewPew::PEWPEW_LIFETIME = 0.5f;
const float PewPew::PEWPEW_SPEED = 290.f;

PewPew::PewPew() : Entity(NULL)
{
	name = "PEWPEW";
	this->size = glm::vec3(2.f, 2.f, 10.f);
	objPath = "../Assets/Models/pewpew.obj";

	damage = 50.f;

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	Initialize();
}

PewPew::~PewPew()
{

}

void PewPew::Update(float dt)
{
	position.z += dt * PEWPEW_SPEED;

	timeElapsed += dt;

	if (timeElapsed > PEWPEW_LIFETIME)
	{
		markedForDeletion = true;
	}
}

void PewPew::OnCollision(Entity* other)
{
	if (other->GetName() == "ENEMY" ||
		other->GetName() == "CUBE")
	{
		markedForDeletion = true;
	}
}