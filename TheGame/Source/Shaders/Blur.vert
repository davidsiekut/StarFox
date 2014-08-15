#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV_modelspace; 
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexColor;
 
uniform mat4 WorldTransform;
uniform mat4 ViewTransform;
uniform mat4 ProjTransform;
 
smooth out vec2 vertOutTexCoords;
 
void main(void)
{
	// Get transformation matrices
	mat4 MV = ViewTransform * WorldTransform;
	mat4 MVP = ProjTransform * MV;

	// Get the position of the model
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

    // Pass the texture coordinates
    vertOutTexCoords = vertexUV_modelspace;
}