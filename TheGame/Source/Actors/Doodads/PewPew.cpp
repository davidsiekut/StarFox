#include "PewPew.h"
#include "Scene.h"
#include "TextureHelper.h"

const float PewPew::PEWPEW_LIFETIME = 0.5f;
const float PewPew::PEWPEW_SPEED_PLAYER = 290.f;
const float PewPew::PEWPEW_SPEED_ENEMY = 25.f;

// Create vertices for the square buffer
const GLfloat squareVertices[] =
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
};

int PewPew::ddsTextureID = -1;

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

	// Get the maximum x and y to create a billboard.
	for (std::vector<Vertex>::iterator it = buffer.begin(); it < buffer.end(); it++)
	{
		if (it == buffer.begin())
		{
			size_x = (*it).position.x * size.x;
			size_y = (*it).position.y * size.y;
		}
		else
		{
			if ((*it).position.x * size.x > size_x)
			{
				this->size_x = (*it).position.x * size.x;
			}
			if ((*it).position.y * size.y > size_y)
			{
				this->size_y = (*it).position.y * size.y;
			}
		}
	}

	if (ddsTextureID == -1)
	{
		ddsTextureID = TextureHelper::LoadDDS("../Assets/Textures/particle.DDS");
	}

	// create vertex array
	glGenVertexArrays(1, &squareArrayID);

	// upload vertexbuffer to the GPU
	glGenBuffers(1, &squareBufferID);
	// and keep a reference to it (vertexBufferID)
	glBindBuffer(GL_ARRAY_BUFFER, squareBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), &squareVertices[0], GL_STATIC_DRAW);
}

PewPew::~PewPew()
{
	glDeleteBuffers(1, &squareBufferID);
	glDeleteVertexArrays(1, &squareBufferID);
}

void PewPew::Draw()
{
	glm::vec3 scale = GetScaling();
	Entity::Draw();

	// Draw the blur effect after
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	BindBuffers(SHADER_BLUR_HORIZONTAL);

	//this->SetScaling(glm::vec3(scale.x, scale.y * 1.15f, scale.z));
	//BindBuffers(SHADER_BLUR_VERTICAL);

	glDisable(GL_BLEND);

	this->SetScaling(scale);
}

void PewPew::BindBuffers(ShaderType shaderType)
{
	GLuint program = Renderer::GetInstance().GetShaderProgramID(shaderType);
	glUseProgram(program);

	glm::mat4 P = Scene::GetInstance().GetGPCamera()->GetProjectionMatrix();
	glm::mat4 V = Scene::GetInstance().GetGPCamera()->GetViewMatrix();
	glm::vec3 position = GetPosition();

	GLuint TextureSamplerID = glGetUniformLocation(program, "MyTextureSampler");
	GLuint CenterPositionID = glGetUniformLocation(program, "center_worldspace");
	GLuint SizeID = glGetUniformLocation(program, "size");
	GLuint ViewMatrixID = glGetUniformLocation(program, "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(program, "ProjTransform");
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(program, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(program, "CameraUp_worldspace");

	glUniform3f(CenterPositionID, position.x, position.y, position.z);
	glUniform2f(SizeID, size_x*2.5f, size_y*2.5f);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);
	glUniform3f(CameraRight_worldspace_ID, V[0][0], V[1][0], V[2][0]);
	glUniform3f(CameraUp_worldspace_ID, V[0][1], V[1][1], V[2][1]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ddsTextureID);
	// Set sampler to user Texture Unit 0
	glUniform1i(TextureSamplerID, 0);

	glBindVertexArray(squareArrayID);

	// Square vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, squareBufferID);
	glVertexAttribPointer(0,    // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                      // size
		GL_FLOAT,               // type
		GL_FALSE,               // normalized?
		0,         // stride
		(void*)0                // array buffer offset
	);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

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