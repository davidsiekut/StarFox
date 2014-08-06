#pragma once

#include "Renderer.h"
#include "glm/glm.hpp"
#include <string>

class Entity
{
public:
	Entity(Entity *parent);
	virtual ~Entity();

	static void Initialize();

	virtual void Update(float dt);
	virtual void Draw();

	virtual glm::mat4 GetWorldMatrix() const;

	void SetPosition(glm::vec3 position);
	void SetScaling(glm::vec3 scaling);
	void SetRotation(glm::vec3 axis, float angle);

	std::string GetName() const { return name; }
	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetScaling() const { return scaling; }
	glm::vec3 GetRotationAxis() const { return rotationAxis; }
	float GetRotationAngle() const { return rotationAngle; }

	glm::vec4 GetMaterialCoefficients() { return materialCoefficients; }

	ShaderType GetShaderType() { return shaderType; }
protected:
	std::string name;
	glm::vec3 position;
	glm::vec3 scaling;
	glm::vec3 rotationAxis;
	float rotationAngle; // in degrees

	Entity *parent;

	glm::vec4 materialCoefficients; //ka, kd, ks, n

	ShaderType shaderType;

private:
};