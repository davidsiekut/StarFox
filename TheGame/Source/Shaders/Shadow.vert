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

#define GROUND_PLANE 0.0f

uniform mat4 ProjTransform;
uniform mat4 ViewTransform;
uniform mat4 WorldTransform;

layout(location = 0) in vec3 vertexPosition_modelspace;

out vec4 v_color;

void main()
{
	vec4 vertexPosition_worldspace = WorldTransform * vec4(vertexPosition_modelspace, 1.0f);
	vertexPosition_worldspace.y =  GROUND_PLANE + 0.1f;
	gl_Position = ProjTransform * ViewTransform * vertexPosition_worldspace;

	v_color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}