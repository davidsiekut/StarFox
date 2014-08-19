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

uniform mat4 ProjTransform;
uniform mat4 ViewTransform;
uniform mat4 WorldTransform;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV_modelspace; 
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexColor; 

out vec3 normal;
out vec4 v_color;

void main()
{
	gl_Position = ProjTransform * ViewTransform * WorldTransform * vec4(vertexPosition_modelspace, 1.0f);

	normal = vertexNormal_modelspace;
	v_color = vec4(vertexColor, 1.0f);
}