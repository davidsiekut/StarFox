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
//
//

#include "Chunk.h"

const float Chunk::CHUNK_DEPTH = 60.f;
const float Chunk::CHUNK_WIDTH = 400.f;

Chunk::Chunk(Entity *parent) : Cube(parent, glm::vec3(CHUNK_WIDTH, 0.1f, CHUNK_DEPTH), glm::vec2(CHUNK_WIDTH / 10, CHUNK_DEPTH / 10))
{
	this->name = "CHUNK";
	this->size = glm::vec3(CHUNK_WIDTH, 0.1f, CHUNK_DEPTH);
	this->objPath = "../Assets/Models/cube.obj";
	this->textureID = 3;
	this->shaderType = SHADER_PHONG_TEXTURED;

	Initialize(size);
}

Chunk::~Chunk()
{

}