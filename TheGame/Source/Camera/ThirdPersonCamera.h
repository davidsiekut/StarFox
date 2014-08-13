#pragma once

#include "Camera.h"
#include "Entity.h"

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(float radiusLength, Entity* parentEntity);
	virtual ~ThirdPersonCamera();
	virtual glm::vec3 GetPosition();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 mPosition;
	glm::vec3 mViewPoint;
	glm::vec3 mUpVector;

	float alphaMouseAngle;
	float betaMouseAngle;

	float cameraMouseSpeed;
	float cameraKeyboardSpeed;

	float radiusLength;
};
