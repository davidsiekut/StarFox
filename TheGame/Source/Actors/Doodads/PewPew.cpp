#include "PewPew.h"

const float PewPew::PEWPEW_LIFETIME = 0.5f;
const float PewPew::PEWPEW_SPEED_PLAYER = 290.f;
const float PewPew::PEWPEW_SPEED_ENEMY = 20.f;

std::vector<Entity::Vertex>* PewPew::bluePrint = new std::vector<Entity::Vertex>();

PewPew::PewPew(std::string owner) : Entity(NULL), owner(owner)
{
	name = "PEWPEW";
	this->size = glm::vec3(2.f, 2.f, 10.f);
	objPath = "../Assets/Models/pewpew.obj";

	if (owner == "PLAYER")
		damage = 50.f;
	else
		damage = 10.f;

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	bluePrint = Initialize(size, bluePrint);
}

PewPew::~PewPew()
{

}

void PewPew::Update(float dt)
{
	if (owner == "PLAYER")
		position.z += dt * PEWPEW_SPEED_PLAYER;
	else
		position.z -= dt * PEWPEW_SPEED_ENEMY;

	timeElapsed += dt;

	if (timeElapsed > PEWPEW_LIFETIME)
	{
		markedForDeletion = true;
	}
}

void PewPew::OnCollision(Entity* other)
{
	if (other->GetName() != owner)
	{
		markedForDeletion = true;
	}
}