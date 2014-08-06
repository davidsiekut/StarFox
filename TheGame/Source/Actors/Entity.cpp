#include "Entity.h"
#include <glm/gtc/matrix_transform.hpp>

Entity::Entity(Entity *parent) :	name("UNNAMED"), 
									position(0.0f, 0.0f, 0.0f), 
									scaling(1.0f, 1.0f, 1.0f), 
									rotationAxis(0.0f, 1.0f, 0.0f), 
									rotationAngle(0.0f), 
									materialCoefficients(0.0, 0.0, 0.0, 1.0),
									shaderType(ShaderType::SHADER_TEST)
{

}

Entity::~Entity()
{

}

void Entity::Initialize()
{

}

void Entity::Update(float dt)
{

}

void Entity::Draw()
{

}

glm::mat4 Entity::GetWorldMatrix() const
{
	glm::mat4 W(1.0f);

	W = glm::translate(W, position);
	W = glm::rotate(W, rotationAngle, rotationAxis);
	W = glm::scale(W, scaling);

	if (parent != NULL)
	{
		W = parent->GetWorldMatrix() * W; // recursive call to get parent matrices
	}

	return W;
}

void Entity::SetPosition(glm::vec3 position)
{
	position = position;
}

void Entity::SetScaling(glm::vec3 scaling)
{
	scaling = scaling;
}

void Entity::SetRotation(glm::vec3 axis, float angle)
{
	rotationAxis = axis;
	rotationAngle = angle;
}