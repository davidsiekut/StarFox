#pragma once

#include "Bloom.h"
#include "TextureHelper.h"
#include "Scene.h"

const GLfloat Bloom::squareVertices[] = 
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f
};

GLuint Bloom::ddsTextureID = -1;

Bloom::Bloom(Entity& parent, float sizeX, float sizeY) : Entity(&parent), sizeX(sizeX), sizeY(sizeY)
{
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

Bloom::~Bloom()
{
	glDeleteBuffers(1, &squareBufferID);
	glDeleteVertexArrays(1, &squareBufferID);
}

void Bloom::Update(float dt)
{
	// Do nothing
}

void Bloom::Draw()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	BindBuffers(SHADER_BLUR_HORIZONTAL);
	BindBuffers(SHADER_BLUR_VERTICAL);

	glDisable(GL_BLEND);
}

void Bloom::BindBuffers(ShaderType shaderType)
{
	GLuint program = Renderer::GetInstance().GetShaderProgramID(shaderType);
	glUseProgram(program);

	glm::mat4 P = Scene::GetInstance().GetGPCamera()->GetProjectionMatrix();
	glm::mat4 V = Scene::GetInstance().GetGPCamera()->GetViewMatrix();
	glm::vec3 position = GetPositionWorld();

	GLuint BloomSamplerID = glGetUniformLocation(program, "bloomSampler");
	GLuint OriginalSamplerID = glGetUniformLocation(program, "originalSampler");
	GLuint CenterPositionID = glGetUniformLocation(program, "center_worldspace");
	GLuint SizeID = glGetUniformLocation(program, "size");
	GLuint ViewMatrixID = glGetUniformLocation(program, "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(program, "ProjTransform");
	GLuint CameraRight_worldspace_ID = glGetUniformLocation(program, "CameraRight_worldspace");
	GLuint CameraUp_worldspace_ID = glGetUniformLocation(program, "CameraUp_worldspace");

	glUniform3f(CenterPositionID, position.x, position.y, position.z);
	glUniform2f(SizeID, sizeX, sizeY);
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);
	glUniform3f(CameraRight_worldspace_ID, V[0][0], V[1][0], V[2][0]);
	glUniform3f(CameraUp_worldspace_ID, V[0][1], V[1][1], V[2][1]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ddsTextureID);
	// Set sampler to user Texture Unit 0
	glUniform1f(BloomSamplerID, 0);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, parent->GetTextureID() + 1); // texture id starts at 1 for opengl
	glUniform1i(OriginalSamplerID, 2);

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