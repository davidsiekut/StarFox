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

uniform mat4 ProjTransform;
uniform mat4 ViewTransform;
uniform mat4 WorldTransform;

uniform vec3 lAttenuation;
uniform vec3 lColor;
uniform vec4 lPosition_World; 
uniform vec4 materialCoefficients;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV_modelspace; 
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexColor; 

out vec2 tex_coord;
smooth out vec3 v_normal;

void main()
{
	gl_Position = ProjTransform * ViewTransform * WorldTransform * vec4(vertexPosition_modelspace, 1.0);

	tex_coord = vertexUV_modelspace;
	v_normal = normalize((transpose(inverse(ViewTransform * WorldTransform)) * vec4(vertexNormal_modelspace, 0.0)).xyz);
}