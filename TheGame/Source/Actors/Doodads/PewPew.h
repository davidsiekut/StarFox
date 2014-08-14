#pragma once

#include "Arwing.h"
#include "Entity.h"

class PewPew : public Entity
{
public:
	PewPew(std::string owner);
	PewPew(std::string owner, glm::vec3 direction);
	~PewPew();

	void Update(float dt);
	virtual void Draw();
	void OnCollision(Entity* other);

	float damage;
	std::string owner;

	static const float PEWPEW_LIFETIME;
	static const float PEWPEW_SPEED_PLAYER;
	static const float PEWPEW_SPEED_ENEMY;

protected:

private:
	float timeElapsed = 0.f;
	glm::vec3 direction;

	unsigned int heightBlurArrayID;
	unsigned int heightBlurBufferID;
	unsigned int heightBlurBufferSize;

	unsigned int widthBlurArrayID;
	unsigned int widthBlurBufferID;
	unsigned int widthBlurBufferSize;

	unsigned int diagonalBlurArrayID;
	unsigned int diagonalBlurBufferID;
	unsigned int diagonalBlurBufferSize;

	void Init();
	void BindBuffers(ShaderType shaderType, int arrayID, int bufferID, int bufferSize);
};