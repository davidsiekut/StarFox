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

#pragma once

#include "Building.h"
#include "FilledMirroredCubes.h"

FilledMirroredCubes::FilledMirroredCubes(Entity* parent, glm::vec2 textureCoords) : Entity(parent),
sides(this, textureCoords),
center(this, glm::vec3(BUILDING_SIZE_X / 2.f, BUILDING_SIZE_Y / 2.f, BUILDING_SIZE_Z), textureCoords)
{
	center.SetPosition(glm::vec3(0.f, BUILDING_SIZE_Y / 4.f, 0.f));
}

void FilledMirroredCubes::Draw()
{
	center.Draw();
	sides.Draw();
}

void FilledMirroredCubes::SetShaderType(ShaderType shaderType)
{
	this->shaderType = shaderType;
	center.SetShaderType(shaderType);
	sides.SetShaderType(shaderType);
}