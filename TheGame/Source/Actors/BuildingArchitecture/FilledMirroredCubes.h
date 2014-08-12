#pragma once

#include "Entity.h"
#include "MirroredCubes.h"
#include "Cube.h"

#include <glm/glm.hpp>

class FilledMirroredCubes : public Entity
{
public:
	FilledMirroredCubes(Entity* parent);

	void SetShaderType(ShaderType shaderType);

	virtual void Draw();

private:
	MirroredCubes sides;
	Cube center;
};