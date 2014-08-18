#pragma once

#include "Cube.h"
#include "Entity.h"

class Chunk : public Cube
{
public:
	Chunk(Entity *parent);
	~Chunk();

	static const float CHUNK_DEPTH;
	static const float CHUNK_WIDTH;

protected:

private:
};