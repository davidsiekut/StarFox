#pragma once

#include "Entity.h"
#include "Cube.h"

class MirroredCubes : public Entity
{
public:
	MirroredCubes(Entity* parent);
	virtual ~MirroredCubes();

	virtual void Draw();

	void SetScaleOfCubes(float x, float y);

private:
	Cube left;
	Cube right;
};