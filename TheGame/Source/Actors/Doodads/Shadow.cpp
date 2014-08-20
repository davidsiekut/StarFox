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
// Quang Tran
//

#pragma once

#include "Shadow.h"

Shadow::Shadow(Entity &parent, std::string objPath) : Entity(&parent)
{
	this->name = "SHADOW";
	this->shaderType = SHADER_SHADOW;
	this->objPath = objPath;

	vertexArrayID = parent.GetVertexArrayID();
	vertexBufferID = parent.GetVertexBufferID();
	vertexBufferSize = parent.GetVertexBufferSize();
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