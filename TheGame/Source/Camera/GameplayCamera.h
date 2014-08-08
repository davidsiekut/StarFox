#pragma once

#include "Camera.h"

class GameplayCamera : public Camera
{
public:
	GameplayCamera(float distance, Entity* parent);
	virtual ~GameplayCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 mPosition;
	glm::vec3 mViewPoint;
	glm::vec3 mUpVector;

	float distance;
};