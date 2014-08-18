#pragma once

#include "Renderer.h"
#include "glm/glm.hpp"
#include <map>
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

	void SetPosition(glm::vec3 position) { this->position = position; }
	void SetScaling(glm::vec3 scaling) { this->scaling = scaling; }
	void SetRotation(glm::vec3 axis, float angle) { this->rotationAxis = axis; this->rotationAngle = angle;	};

	void SetTextureID(int id) { textureID = id; }
	void SetTextureCoords(glm::vec2 textureCoordinates) { this->textureCoordinates = textureCoordinates; }
	void SetShaderType(ShaderType type) { shaderType = type; }
	void SetMarkedForDeletion() { markedForDeletion = true; }

	float GetShieldAmount() { return shield; }
	virtual bool IsOpaque() { return true; }
	bool IsMarkedForDeletion() { return markedForDeletion; }
	bool IsFlashing() { return isFlashing; }

	std::string GetName() const { return name; }
	glm::vec3 GetPosition() const { return position; }
	glm::vec3 GetPositionWorld();
	glm::vec3 GetScaling() const { return scaling; }
	glm::vec3 GetRotationAxis() const { return rotationAxis; }
	float GetRotationAngle() const { return rotationAngle; }
	glm::vec3 GetSize() const { return size; }
	glm::vec4 GetMaterialCoefficients() const { return materialCoefficients; }
	ShaderType GetShaderType() const { return shaderType; }
	unsigned int GetTextureID() const { return textureID; }
	float GetRadius();

	// dimensions of box collider for this entity
	float COLLIDE_X;
	float COLLIDE_Y;
	float COLLIDE_Z;
	void TakeDamage(float f);

protected:
	void Initialize(glm::vec3 size);

	Entity *parent;
	std::string name;

	glm::vec3 position;
	glm::vec3 scaling;
	glm::vec3 size; // use this instead of scaling when loading an entity to prevent parent-child shearing
	glm::vec3 rotationAxis;
	float rotationAngle; // in degrees

	std::string objPath;
	unsigned int textureID;
	glm::vec2 textureCoordinates;
	ShaderType shaderType;
	glm::vec4 materialCoefficients; //ka, kd, ks, n

	float shield;
	bool hasShadow;
	bool markedForDeletion;
	bool isFlashing;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec2 uv;
		glm::vec3 normal;
		glm::vec3 color;
	};

	std::vector<Vertex> LoadVertices();

private:
	unsigned int vertexArrayID;
	unsigned int vertexBufferID;
	unsigned int vertexBufferSize;
	unsigned int vertexArrayBloomID;
	unsigned int vertexBufferBloomID;
	unsigned int currentVertexArrayID;
	unsigned int currentVertexBufferID;

	Entity* shadow;

	bool loadOBJ(std::string path, std::vector<Vertex> &buffer);

	// Hold the blueprints of each Entity in memory once the .obj has been loaded.
	static std::map<std::string, std::vector<Vertex>*> bluePrints;
};