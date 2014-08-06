#pragma once

#include "Entity.h"

class Entity;

class Scene
{
public:
	static void Initialize();
	void Update(float dt);
	void Draw();

private:
	std::vector<Entity*> entities;
	//glm::vec4 lightPosition;
	//std::vector<Light*> lights;

	GLuint WorldMatrixID;
	GLuint ViewMatrixID;
	GLuint ProjMatrixID;

	GLuint LightPositionID;
	GLuint LightColorID;
	GLuint LightAttenuationID;

	GLuint MaterialID;
};