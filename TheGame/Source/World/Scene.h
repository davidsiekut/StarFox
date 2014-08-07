#pragma once

#include "Entity.h"
#include "ThirdPersonCamera.h"

class Entity;

class Scene
{
public:
	void Initialize();
	void Update(float dt);
	void Draw();

	void AddEntity(Entity* entity);

private:
	std::vector<Entity*> entities;
	//glm::vec4 lightPosition;
	//std::vector<Light*> lights;
};