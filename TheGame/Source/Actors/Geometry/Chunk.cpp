#include "Chunk.h"

const float Chunk::CHUNK_DEPTH = 60.f;
const float Chunk::CHUNK_WIDTH = 300.f;

std::vector<Entity::Vertex>* Chunk::bluePrint = new std::vector<Entity::Vertex>();

Chunk::Chunk(Entity *parent) : Entity(parent)
{
	name = "CHUNK";
	size = glm::vec3(CHUNK_WIDTH, 0.1f, CHUNK_DEPTH);
	objPath = "../Assets/Models/cube.obj";

	bluePrint = Initialize(size, bluePrint);
}

Chunk::~Chunk()
{
	//printf("[Cleanup] Chunk deleted\n");
}

void Chunk::Update(float dt)
{

}