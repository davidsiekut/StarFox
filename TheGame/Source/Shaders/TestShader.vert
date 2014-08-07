#version 330 core

// Vertex data
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec3 vertexColor; 

out vec3 v_Color;

// Model Transformations
uniform mat4 WorldTransform;
uniform mat4 ViewTransform;
uniform mat4 ProjTransform;

// Lighting
uniform vec4 lPosition_World;
uniform vec3 lColor;
uniform vec3 lAttenuation;
uniform vec4 materialCoefficients;


void main()
{
	//vec4 a = gl_Vertex;
	//a.x = a.x * 0.5;
	//a.y = a.y * 0.5;

	//gl_Position = gl_ModelViewProjectionMatrix * a;

	mat4 MVP = ProjTransform*ViewTransform*WorldTransform;
	gl_Position = MVP * vec4(vertexPosition_modelspace,1);

	v_Color = vertexColor;
}  