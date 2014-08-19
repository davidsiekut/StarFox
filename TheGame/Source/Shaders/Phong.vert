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
// David Siekut
//

#version 330 core

uniform mat4 WorldTransform;
uniform mat4 ViewTransform;
uniform mat4 ProjTransform;

uniform vec3 lAttenuation;
uniform vec3 lColor;
uniform vec4 lPosition_World; 
uniform vec4 materialCoefficients;

layout(location = 0) in vec3 vertexPosition_Model;
layout(location = 1) in vec2 vertexUV_Model;
layout(location = 2) in vec3 vertexNormal_Model;
layout(location = 3) in vec3 vertexColor;

out vec3 v_color;
out vec3 normalVector;
out vec3 eyeVector;
out vec4 lightVector;

void main()
{
	mat4 MV = ViewTransform * WorldTransform;
	mat4 MVP = ProjTransform * MV;

	gl_Position =  MVP * vec4(vertexPosition_Model, 1.0);
	v_color = vertexColor;

	normalVector = (transpose(inverse(MV)) * vec4(vertexNormal_Model, 1.0)).xyz;

	vec3 vertexPosition_View = (MV * vec4(vertexPosition_Model, 1.0)).xyz;
	eyeVector = -vertexPosition_View;

	vec4 lPosition_View = ViewTransform * lPosition_World;
	if (lPosition_World.w == 0.0)
	{
		lightVector = -lPosition_View;
	}
	else
	{
		lightVector = lPosition_View - vec4(vertexPosition_View, 0.0);
	}
}