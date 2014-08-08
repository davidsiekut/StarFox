#include "Chunk.h"

const float Chunk::CHUNK_DEPTH = 60.f;
const float Chunk::CHUNK_WIDTH = 300.f;

Chunk::Chunk(Entity *parent) : Entity(parent)
{
	name = "CHUNK";
	size = glm::vec3(CHUNK_WIDTH, 0.1f, CHUNK_DEPTH);
	//shaderType = SHADER_SOLID_COLOR;
	shaderType = SHADER_GOURAUD;

	objPath = "../Assets/Models/cube.obj";

	Initialize();
}

Chunk::~Chunk()
{

}

void Chunk::Update(float dt)
{

}