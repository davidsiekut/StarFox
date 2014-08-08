#pragma once

#include <GLM/glm.hpp>
#include <Entity.h>

class Camera
{
public:
	Camera();
	Camera(Entity* parentEntity);
	virtual ~Camera();

	virtual void Update(float dt) = 0;

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewProjectionMatrix() const;

protected:

	// The camera will follow this entity
	Entity* parent;

};
