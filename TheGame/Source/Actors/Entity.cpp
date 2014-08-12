#include "Entity.h"
#define GLM_FORCE_RADIANS
#define dtor(x) x*(3.141592f/180.0f)
#include <glm/gtc/matrix_transform.hpp>

// TEMPORARY
#include <time.h>

std::map<std::string, std::vector<Entity::Vertex>*> Entity::bluePrints = std::map<std::string, std::vector<Vertex>*>();

Entity::Entity(Entity *parent) :	name("UNNAMED"),
									parent(parent),
									position(0.0f, 0.0f, 0.0f),
									scaling(1.0f, 1.0f, 1.0f),
									size(1.0f, 1.0f, 1.0f),
									rotationAxis(0.0f, 1.0f, 0.0f),
									rotationAngle(0.0f),
									materialCoefficients(0.2f, 0.8f, 0.2f, 50.0f),
									shaderType(ShaderType::SHADER_SOLID_COLOR),
									objPath(""),
									textureID(0),
									markedForDeletion(false)
{
	
}

Entity::~Entity()
{
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Entity::Initialize(glm::vec3 size)
{
	std::vector<Vertex> buffer;

	if(bluePrints.find(objPath) != bluePrints.end())
	{
		buffer = *bluePrints[objPath];
	}
	else
	{
		bool res = loadOBJ(objPath, buffer);
		bluePrints.insert(std::pair<std::string, std::vector<Vertex>*>(objPath, &buffer));
		printf("[Entity] Stored %s into memory\n", objPath.c_str());
	}

	for (std::vector<Vertex>::iterator it = buffer.begin(); it < buffer.end(); it++)
	{
		(*it).position.x *= size.x;
		(*it).position.y *= size.y;
		(*it).position.z *= size.z;
	}

	// if Vertex struct is modified, this needs to be changed also
	vertexBufferSize = buffer.size();
	//printf("%i", vertexBufferSize);

	// create vertex array
	glGenVertexArrays(1, &vertexArrayID);

	// upload vertexbuffer to the GPU
	glGenBuffers(1, &vertexBufferID);
	// and keep a reference to it (vertexBufferID)
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * (3 * sizeof(glm::vec3) + sizeof(glm::vec2)), &buffer[0], GL_STATIC_DRAW);
}

void Entity::Update(float dt)
{

}

glm::vec3 Entity::GetPositionWorld()
{
	glm::vec3 pos = GetPosition();

	if (parent != NULL)
	{
		pos = parent->GetPosition() + pos;
	}

	return pos;
}

void Entity::Draw()
{
	GLuint program = Renderer::GetShaderProgramID(this->shaderType);

	glm::mat4 W = GetWorldMatrix();
	GLuint WorldMatrixID = glGetUniformLocation(program, "WorldTransform");
	glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &W[0][0]);

	GLuint materialCoefficientsID = glGetUniformLocation(program, "materialCoefficients");
	glUniform4f(materialCoefficientsID, materialCoefficients.x, materialCoefficients.y, materialCoefficients.z, materialCoefficients.w);

	glBindVertexArray(vertexArrayID);

	// position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0,    // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                      // size
		GL_FLOAT,               // type
		GL_FALSE,               // normalized?
		sizeof(Vertex),         // stride
		(void*)0                // array buffer offset
		);

	// uv
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(glm::vec3) // offset
		);

	// normal
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)) // offset
		);


	// color
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(3,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2*sizeof(glm::vec3) + sizeof(glm::vec2)) // offset
		);

	glDrawArrays(GL_TRIANGLES, 0, vertexBufferSize);

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

glm::mat4 Entity::GetWorldMatrix() const
{
	glm::mat4 W(1.0f);

	W = glm::translate(W, position);
	W = glm::rotate(W, dtor(rotationAngle), rotationAxis);
	W = glm::scale(W, scaling);

	if (parent != NULL)
	{
		W = parent->GetWorldMatrix() * W; // recursive call to get parent matrices
	}

	return W;
}

void Entity::SetPosition(glm::vec3 position)
{
	this->position = position;
}

void Entity::SetScaling(glm::vec3 scaling)
{
	this->scaling = scaling;
}

void Entity::SetRotation(glm::vec3 axis, float angle)
{
	this->rotationAxis = axis;
	this->rotationAngle = angle;
}

bool Entity::loadOBJ(std::string path, std::vector<Entity::Vertex> &buffer)
{
	//http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	FILE * file = fopen(path.c_str(), "r");
	if (file == NULL)
	{
		printf("[Entity] Error in loadOBJ() fopen()\n");
		return false;
	}

	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			fclose(file);
			break; // EOF = End Of File. Quit the loop.
		}

		// else : parse lineHeader
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			// rescale all vertices based on predefined size
			vertex.x = vertex.x;
			vertex.y = vertex.y;
			vertex.z = vertex.z;
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("[Entity] Invalid file contents. Please ensure .obj has been exported properly.\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	srand((int)time(NULL));
	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];

		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];

		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		Vertex v = Vertex();
		v.position = vertex;
		v.uv = uv;
		v.normal = normal;

		// give our vertex a random colour
		// maybe change this when uv tex mapping is added
		float r = (rand() / (float)(RAND_MAX + 1));
		float g = (rand() / (float)(RAND_MAX + 1));
		float b = (rand() / (float)(RAND_MAX + 1));
		v.color = glm::vec3(r, g, b);

		buffer.push_back(v);
	}

	return true;
}

void Entity::OnCollision(Entity* other)
{

}

void Entity::TakeDamage(float f)
{
	shield -= f;
}