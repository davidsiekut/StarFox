#pragma once

#include "Renderer.h"
#include <map>
#include "glm/glm.hpp"
#include <string>

class Entity
{
public:
	Entity(Entity *parent);
	virtual ~Entity();

	virtual void Update(float dt);
	virtual void Draw();

	virtual void OnCollision(Entity* other);

	glm::mat4 GetWorldMatrix() const;

	void SetPosition(glm::vec3 position);
	void SetScaling(glm::vec3 scaling);
	void SetRotation(glm::vec3 axis, float angle);
	void SetShaderType(ShaderType type) { shaderType = type; }
	void SetTextureID(int id) { textureID = id; }
	void SetTextureCoords(glm::vec2 textureCoordinates) { this->textureCoordinates = textureCoordinates; }

	std::string GetName() const { return name; }
	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetPositionWorld();
	glm::vec3 GetScaling() const { return scaling; }
	glm::vec3 GetRotationAxis() const { return rotationAxis; }
	float GetRotationAngle() const { return rotationAngle; }
	glm::vec3 GetSize() { return size; }
	glm::vec4 GetMaterialCoefficients() { return materialCoefficients; }
	ShaderType GetShaderType() { return shaderType; }
	unsigned int GetTextureID() { return textureID; }
	virtual bool IsOpaque() { return true; }

	float GetRadius();

	// dimensions of box collider for this entity
	float COLLIDE_X;
	float COLLIDE_Y;
	float COLLIDE_Z;

	float GetShieldAmount() { return shield; };
	void TakeDamage(float f);
	float invicibilityFrames = 0.f;
	bool markedForDeletion;
	bool isFlashing;

protected:
	Entity *parent;
	std::string name;
	glm::vec3 position;
	glm::vec3 scaling;
	glm::vec3 size; // use this instead of scaling to prevent parent-child shearing
	glm::vec2 textureCoordinates;
	glm::vec3 rotationAxis;
	float rotationAngle; // in degrees
	bool hasShadow;

	glm::vec4 materialCoefficients; //ka, kd, ks, n
	ShaderType shaderType;
	std::string objPath;
	unsigned int textureID;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 color;
	};

	void Initialize(glm::vec3 size);

	// gameplay stuff
	float shield = 100.f;

	std::vector<Vertex> LoadVertices();

private:
	unsigned int vertexArrayID;
	unsigned int vertexBufferID;
	unsigned int vertexBufferSize;

	unsigned int vertexArrayBloomID;
	unsigned int vertexBufferBloomID;

	unsigned int currentVertexArrayID;
	unsigned int currentVertexBufferID;

	bool loadOBJ(std::string path, std::vector<Vertex> &buffer);
	Entity* shadow;

	// Hold the blueprints of each Entity in memory once the .obj has been loaded.
	static std::map<std::string, std::vector<Vertex>*> bluePrints;
};