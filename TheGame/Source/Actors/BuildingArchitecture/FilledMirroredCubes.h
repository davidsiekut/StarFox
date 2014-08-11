#pragma once

#include "Entity.h"
#include "MirroredCubes.h"
#include "Cube.h"

#include <glm/glm.hpp>

class FilledMirroredCubes : public Entity
{
public:
	FilledMirroredCubes(Entity* parent);

	virtual void Draw();

private:
	MirroredCubes sides;
	Cube center;
};