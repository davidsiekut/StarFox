#pragma once

#include "Shadow.h"

Shadow::Shadow(Entity &parent, std::string objPath) : Entity(&parent)
{
	name = "SHADOW";
	shaderType = SHADER_SHADOW;
	this->objPath = objPath;

	Initialize(parent.GetSize());
}

void Shadow::Update(float dt)
{
	// Do nothing
}

void Shadow::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Entity::Draw();

	glDisable(GL_BLEND);
}