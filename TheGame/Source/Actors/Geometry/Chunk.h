#pragma once

#include "Entity.h"

class Chunk : public Entity
{
public:
	Chunk(Entity *parent);
	~Chunk();
	void Update(float dt);

	static const float CHUNK_DEPTH;
	static const float CHUNK_WIDTH;

protected:

private:
};