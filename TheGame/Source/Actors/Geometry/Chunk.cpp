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