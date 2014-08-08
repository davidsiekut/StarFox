#pragma once

#include "Camera.h"
#include "Entity.h"

class ThirdPersonCamera : public Camera
{
public:
	ThirdPersonCamera(float radiusLength, Entity* parentEntity);
	virtual ~ThirdPersonCamera();

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;

private:
	glm::vec3 mPosition;
	glm::vec3 mViewPoint;
	glm::vec3 mUpVector;

	float cameraMouseSpeed;
	float cameraKeyboardSpeed;

	float alphaMouseAngle;
	float betaMouseAngle;

	float radiusLength;
};
