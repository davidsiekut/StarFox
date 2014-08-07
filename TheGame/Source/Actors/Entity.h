#pragma once

#include "Renderer.h"
#include "glm/glm.hpp"
#include <string>

class Entity
{
public:
	Entity(Entity *parent);
	virtual ~Entity();

	void Initialize();

	virtual void Update(float dt);
	void Draw();

	glm::mat4 GetWorldMatrix() const;

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
	Entity *parent;
	std::string name;
	glm::vec3 position;
	glm::vec3 scaling;
	glm::vec3 rotationAxis;
	float rotationAngle; // in degrees

	glm::vec4 materialCoefficients; //ka, kd, ks, n
	ShaderType shaderType;
	std::string objPath;

private:
	unsigned int vertexArrayID;
	unsigned int vertexBufferID;
	unsigned int vertexBufferSize;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 color;
	};

	bool loadOBJ(std::string path, std::vector<Vertex> &buffer);
};