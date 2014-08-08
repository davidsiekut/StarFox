#pragma once

#include "Arwing.h"
#include "Entity.h"
#include "../Camera/ThirdPersonCamera.h"

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
	ThirdPersonCamera* camera;
	//glm::vec4 lightPosition;
	//std::vector<Light*> lights;
	Arwing* a;

	int lastChunk = 0;
};