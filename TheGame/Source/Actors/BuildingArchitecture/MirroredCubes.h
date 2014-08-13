#pragma once

#include "Entity.h"
#include "Cube.h"

class MirroredCubes : public Entity
{
public:
	MirroredCubes(Entity* parent, glm::vec2 textureCoordinates);
	virtual ~MirroredCubes();

	void SetShaderType(ShaderType shaderType);

	virtual void Draw();

private:
	Cube left;
	Cube right;
};