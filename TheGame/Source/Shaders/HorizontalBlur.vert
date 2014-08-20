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
// Dror Ozgaon
//

#version 330 core

layout(location = 0) in vec3 squareVertices;
 
// Values that stay constant for the whole mesh.
uniform vec3 center_worldspace;
uniform vec2 size;
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

// Transform matrices
uniform mat4 ProjTransform;
uniform mat4 ViewTransform;
 
smooth out vec2 vertOutTexCoords;
 
void main(void)
{
	vec3 vertexPosition_worldspace = 
		center_worldspace
			+ CameraRight_worldspace * squareVertices.x * size.x
			+ CameraUp_worldspace * squareVertices.y * size.y;

	// Get the position of the model
	gl_Position =  ProjTransform * ViewTransform * vec4(vertexPosition_worldspace,1);

    // Pass the texture coordinates
    vertOutTexCoords = squareVertices.xy + vec2(0.5, 0.5);
}