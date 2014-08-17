#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 squareVertices;
layout(location = 1) in vec4 xyzs; // Position of the center of the particle and size of the square
layout(location = 2) in vec4 color; // Color of the particle

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 particlecolor;

// Values that stay constant for the whole mesh.
uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

// Transform matrices
uniform mat4 ProjTransform;
uniform mat4 ViewTransform;

void main()
{
	float particleSize = xyzs.w; // because we encoded it this way.
	vec3 particleCenter_wordspace = xyzs.xyz;
	
	// Billboard that always faces the camera
	vec3 vertexPosition_worldspace =
		particleCenter_wordspace
		+ CameraRight_worldspace * squareVertices.x * particleSize
		+ CameraUp_worldspace * squareVertices.y * particleSize;

	// Output position of the vertex
	gl_Position = ProjTransform * ViewTransform * vec4(vertexPosition_worldspace, 1.0f);

	// UV of the vertex.
	UV = squareVertices.xy + vec2(0.5, 0.5);
	particlecolor = color;
}

