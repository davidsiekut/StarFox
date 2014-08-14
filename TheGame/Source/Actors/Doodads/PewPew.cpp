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
	this->textureID = 1;

	Entity::Initialize(size);

	std::vector<Vertex> buffer = Entity::LoadVertices();

	// create vertex array
	glGenVertexArrays(1, &heightBlurBufferID);

	// upload vertexbuffer to the GPU
	glGenBuffers(1, &heightBlurBufferID);
	// and keep a reference to it (vertexBufferID)
	glBindBuffer(GL_ARRAY_BUFFER, heightBlurBufferID);
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * (3 * sizeof(glm::vec3) + sizeof(glm::vec2)), &buffer[0], GL_STATIC_DRAW);

	// create vertex array
	glGenVertexArrays(1, &widthBlurBufferID);

	// upload vertexbuffer to the GPU
	glGenBuffers(1, &widthBlurBufferID);
	// and keep a reference to it (vertexBufferID)
	glBindBuffer(GL_ARRAY_BUFFER, widthBlurBufferID);
	glBufferData(GL_ARRAY_BUFFER, buffer.size() * (3 * sizeof(glm::vec3) + sizeof(glm::vec2)), &buffer[0], GL_STATIC_DRAW);
}

PewPew::~PewPew()
{
	glDeleteBuffers(1, &heightBlurBufferID);
	glDeleteVertexArrays(1, &heightBlurBufferID);

	glDeleteBuffers(1, &widthBlurBufferID);
	glDeleteVertexArrays(1, &widthBlurBufferID);
}

void PewPew::Draw()
{
	Entity::Draw();
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