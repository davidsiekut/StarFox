#include "PewPew.h"
#include "Scene.h"

const float PewPew::PEWPEW_LIFETIME = 0.5f;
const float PewPew::PEWPEW_SPEED_PLAYER = 290.f;
const float PewPew::PEWPEW_SPEED_ENEMY = 25.f;

PewPew::PewPew(std::string owner) : Entity(NULL), owner(owner)
{
	if (owner == "PLAYER")
	{
		direction = glm::vec3(0.f, 0.f, 1.f);
	}
	else
	{
		direction = glm::vec3(0.f, 0.f, -1.f);
	}
	Init();
}

PewPew::PewPew(std::string owner, glm::vec3 direction) : Entity(NULL), owner(owner), direction(direction)
{
	Init();
}

void PewPew::Init()
{
	name = "PEWPEW";

	if (owner == "PLAYER")
	{
		this->size = glm::vec3(2.f, 2.f, 10.f);
		objPath = "../Assets/Models/pewpew.obj";
		damage = 50.f;
	}
	else
	{
		this->size = glm::vec3(0.5f, 0.5f, 0.5f);
		objPath = "../Assets/Models/sphere.obj";
		damage = 10.f;
	}

	COLLIDE_X = size.x;
	COLLIDE_Y = size.y;
	COLLIDE_Z = size.z;

	shaderType = SHADER_BLOOM;
	this->textureID = 6;

	Entity::Initialize(size);

	std::vector<Vertex> buffer = Entity::LoadVertices();
	for (std::vector<Vertex>::iterator it = buffer.begin(); it < buffer.end(); it++)
	{
		(*it).position.x *= size.x;
		(*it).position.y *= size.y;
		(*it).position.z *= size.z;

		(*it).uv.x *= textureCoordinates.x;
		(*it).uv.y *= textureCoordinates.y;
	}

	blurBufferSize = buffer.size();

	// create vertex array
	glGenVertexArrays(1, &blurArrayID);

	// upload vertexbuffer to the GPU
	glGenBuffers(1, &blurBufferID);
	// and keep a reference to it (vertexBufferID)
	glBindBuffer(GL_ARRAY_BUFFER, blurBufferID);
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * (3 * sizeof(glm::vec3) + sizeof(glm::vec2)), &buffer[0], GL_STATIC_DRAW);
}

PewPew::~PewPew()
{
	glDeleteBuffers(1, &blurBufferID);
	glDeleteVertexArrays(1, &blurBufferID);
}

void PewPew::Draw()
{
	glm::vec3 scale = GetScaling();
	Entity::Draw();

	// Draw the blur effect after
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	this->SetScaling(glm::vec3(scale.x * 1.15f, scale.y * 1.15f, scale.z));
	BindBuffers(SHADER_BLUR, blurArrayID, blurBufferID, blurBufferSize);

	glDisable(GL_BLEND);

	this->SetScaling(scale);
}

void PewPew::BindBuffers(ShaderType shaderType, int arrayID, int bufferID, int bufferSize)
{
	glm::mat4 P = Scene::GetInstance().GetGPCamera()->GetProjectionMatrix();
	glm::mat4 V = Scene::GetInstance().GetGPCamera()->GetViewMatrix();
	glm::mat4 W = GetWorldMatrix();

	GLuint program = Renderer::GetInstance().GetShaderProgramID(shaderType);
	glUseProgram(program);

	GLuint ViewMatrixID = glGetUniformLocation(program, "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(program, "ProjTransform");
	GLuint WorldMatrixID = glGetUniformLocation(program, "WorldTransform");
	glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &W[0][0]);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

	GLuint ScaleID = glGetUniformLocation(program, "scale");
	glUniform2f(ScaleID, 1.0f/10.f, 1.0f/10.f);

	glBindVertexArray(arrayID);

	// position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(0,    // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                      // size
		GL_FLOAT,               // type
		GL_FALSE,               // normalized?
		sizeof(Vertex),         // stride
		(void*)0                // array buffer offset
		);

	// uv
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(glm::vec3) // offset
		);

	// normal
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(sizeof(glm::vec3) + sizeof(glm::vec2)) // offset
		);


	// color
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);
	glVertexAttribPointer(3,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)) // offset
		);

	glDrawArrays(GL_TRIANGLES, 0, bufferSize);

	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

}

void PewPew::Update(float dt)
{
	if (owner == "PLAYER")
		position += direction * dt * PEWPEW_SPEED_PLAYER;
	else
		position += direction * dt * PEWPEW_SPEED_ENEMY;

	timeElapsed += dt;

	// Player pewpews have a lifetime. Enemy pewpews will be deleted when they go behind the player.
	if ((timeElapsed > PEWPEW_LIFETIME && owner == "PLAYER") || Scene::GetInstance().IsGameOver())
	{
		markedForDeletion = true;
	}
}

void PewPew::OnCollision(Entity* other)
{
	if (other->GetName() != owner && other->GetName() != "PEWPEW")
	{
		markedForDeletion = true;
	}
}