#pragma once

#include "Camera.h"

class GameplayCamera : public Camera
{
public:
	GameplayCamera(float distance, Entity* parent);
	~GameplayCamera();
	void Update(float dt);

	glm::vec3 GetPosition();
	glm::mat4 GetViewMatrix() const;
	void Shake();

private:
	float distance;
	glm::vec3 position;
	glm::vec3 viewPoint;
	glm::vec3 upVector;

	float timeElapsed = 0.0f;
	float amplitude = 10.0f;
	float amplitudeDamping = -1.0f;
	float xBounce = 1.f;
	float yBounce = 1.1f;
	bool isShaking = false;
};