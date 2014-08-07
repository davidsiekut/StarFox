#include "Cube.h"
#include <gl/glew.h>

using namespace glm;

Cube::Cube(Entity* parent) : Entity(parent)
{
	// Create Vertex Buffer for all the verices of the Cube
	vec3 halfSize = vec3(1.0f, 1.0f, 1.0f) * 0.5f;

	Vertex vertexBuffer[] = {  // position,                normal,                  color
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) }, //left - red
		{ vec3(-halfSize.x, -halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) },
		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) },

		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) },
		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) },
		{ vec3(-halfSize.x, halfSize.y, -halfSize.z), vec3(-1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f) },

		{ vec3(halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f) }, // far - blue
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f) },
		{ vec3(-halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f) },

		{ vec3(halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f) },
		{ vec3(halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f) },
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, 1.0f) },

		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f) }, // bottom - turquoise
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f) },
		{ vec3(halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f) },

		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f) },
		{ vec3(-halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f) },
		{ vec3(-halfSize.x, -halfSize.y, -halfSize.z), vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, 1.0f, 1.0f) },

		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) }, // near - green
		{ vec3(-halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) },
		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) },

		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) },
		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) },
		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 1.0f, 0.0f) },

		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f) }, // right - purple
		{ vec3(halfSize.x, -halfSize.y, -halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f) },
		{ vec3(halfSize.x, halfSize.y, -halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f) },

		{ vec3(halfSize.x, -halfSize.y, -halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f) },
		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f) },
		{ vec3(halfSize.x, -halfSize.y, halfSize.z), vec3(1.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 1.0f) },

		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) }, // top - yellow
		{ vec3(halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) },
		{ vec3(-halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) },

		{ vec3(halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) },
		{ vec3(-halfSize.x, halfSize.y, -halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) },
		{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 0.0f) }
	};

	// Create a vertex array
	glGenVertexArrays(1, &vertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
}


Cube::~Cube()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteVertexArrays(1, &vertexArrayID);
}

void Cube::Draw()
{
	Entity::Draw();

	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(vertexArrayID);

	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(0,				// attribute. No particular reason for 0, but must match the layout in the shader.
		3,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		sizeof(Vertex), // stride
		(void*)0        // array buffer offset
		);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
		);


	// 3rd attribute buffer : vertex color
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
		);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices: 3 * 2 * 6 (3 per triangle, 2 triangles per face, 6 faces)

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}
