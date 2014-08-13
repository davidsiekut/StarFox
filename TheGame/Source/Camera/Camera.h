#pragma once

#include <GLM/glm.hpp>
#include <Entity.h>

class Camera
{
public:
	Camera();
	Camera(Entity* parent);
	virtual ~Camera();

	virtual void Update(float dt) = 0;
	virtual void Shake() = 0;

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const;

protected:
	Entity* parent;	// camera will follow this entity
	bool isShaking = false;
};
