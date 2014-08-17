#pragma once

#include "Entity.h"

class Bloom : public Entity
{
public:
	Bloom(Entity& parent, float sizeX, float sizeY);
	~Bloom();

	virtual void Update(float dt);
	virtual void Draw();

	virtual bool IsOpaque() { return false; }

private:
	static const GLfloat squareVertices[];
	static GLuint ddsTextureID;

	unsigned int squareArrayID;
	unsigned int squareBufferID;

	float sizeX;
	float sizeY;

	void BindBuffers(ShaderType shaderType);
};