#pragma once

#include "Entity.h"
#include <GLM/glm.hpp>

#define CAM_FOV 45.f
#define CAM_ASPECT_X 4.f
#define CAM_ASPECT_Y 3.f
#define CAM_NEAR_CLIP 0.1f
#define CAM_FAR_CLIP 800.f

class Camera
{
public:
	Camera();
	Camera(Entity* parent);
	virtual ~Camera();

	virtual void Update(float dt) = 0;
	virtual glm::vec3 GetPosition() = 0;

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const;

protected:
	Entity* parent;	// camera will follow this entity
};
