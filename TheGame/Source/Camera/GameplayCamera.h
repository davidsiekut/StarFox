#pragma once

#include "Camera.h"

class GameplayCamera : public Camera
{
public:
	GameplayCamera(float distance, Entity* parent);
	virtual ~GameplayCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

	void Shake();

private:
	glm::vec3 mPosition;
	glm::vec3 mViewPoint;
	glm::vec3 mUpVector;

	float distance;
	float timeElapsed = 0.0f;
	float amplitude = 10.0f;
	float amplitudeDamping = -1.0f;
	float xBounce = 1.f;
	float yBounce = 1.1f;
	bool isShaking = false;
};