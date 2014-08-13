#include "Chunk.h"

const float Chunk::CHUNK_DEPTH = 60.f;
const float Chunk::CHUNK_WIDTH = 400.f;

Chunk::Chunk(Entity *parent) : Cube(parent, glm::vec3(CHUNK_WIDTH, 0.1f, CHUNK_DEPTH), glm::vec2(CHUNK_WIDTH / 10, CHUNK_DEPTH / 10))
{
	name = "CHUNK";
	size = glm::vec3(CHUNK_WIDTH, 0.1f, CHUNK_DEPTH);
	objPath = "../Assets/Models/cube.obj";
	shaderType = SHADER_PHONG_TEXTURED;
	textureID = 3;

	Initialize(size);
}

Chunk::~Chunk()
{
	//printf("[Cleanup] Chunk deleted\n");
}

void Chunk::Update(float dt)
{

}