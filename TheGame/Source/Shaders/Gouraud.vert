//
// COMP 371 Term Project
//
// Created by
// Boutas, Vasiliki   (6220304)
// Di Girolamo, John  (6202918)
// Ozgaon, Dror Asher (6296742)
// Siekut, David      (6329810)
// Tran, Quang        (6339816)
// Wan, Kwok - Chak   (6291643)
//
// Contributions to this file:
//
//

#version 330 core

uniform mat4 WorldTransform;
uniform mat4 ViewTransform;
uniform mat4 ProjTransform;

uniform vec3 lAttenuation;
uniform vec3 lColor;
uniform vec4 lPosition_World; 
uniform vec4 materialCoefficients;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV_modelspace; 
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexColor;

out vec3 v_color;

void main()
{
	mat4 MV = ViewTransform * WorldTransform;
	mat4 MVP = ProjTransform * MV;

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	vec4 vertexPosition_viewspace = MV * vec4(vertexPosition_modelspace,1);

	// The normal vector in world space
	vec3 vector_N = normalize((transpose(inverse(MV)) * vec4(vertexNormal_modelspace,0)).xyz);

	// Get the direction towards the light source
	vec3 vector_L;
	float attenuation;
	if(lPosition_World.w == 1)
	{
		vector_L = ((ViewTransform*lPosition_World) - vertexPosition_viewspace).xyz;
		float d = length(vector_L);
		attenuation = 1/(lAttenuation.x + (lAttenuation.y * d) + (lAttenuation.z * d * d)); 
		vector_L = normalize(vector_L);
	}
	else
	{
		vector_L = normalize(-(ViewTransform * lPosition_World).xyz);
		attenuation = 1;
	}

	// Implement Gouraud shading here...
	vec3 ambient = (materialCoefficients.x * vertexColor);
	
	vec3 diffuse = attenuation * lColor * vertexColor * materialCoefficients.y * max(dot(vector_L, vector_N), 0);

	// The camera is the origin in view space
	vec3 vector_V = normalize(-vertexPosition_viewspace.xyz);

	// Reflect the L vector
	vec3 vector_R = normalize(2*(dot(vector_N, vector_L))*vector_N - vector_L);

	vec3 specular = attenuation * lColor * materialCoefficients.z * pow(max(dot(vector_V, vector_R), 0), materialCoefficients.w);

	// This should be the final color on the Vertex, including the lighting
	v_color = ambient + diffuse + specular;
}
