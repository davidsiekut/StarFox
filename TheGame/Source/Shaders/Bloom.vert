#version 330 core

// Attributes per vertex: position, normal and texture coordinates
//in vec4 vertex;
//in vec3 normal;
//in vec2 texCoord0;

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV_modelspace; 
layout(location = 2) in vec3 vertexNormal_modelspace;
layout(location = 3) in vec3 vertexColor;
 
//uniform mat4 MVP;
//uniform mat4 modelview;
//uniform vec3 lightPos;
//uniform vec4 color;

uniform mat4 WorldTransform;
uniform mat4 ViewTransform;
uniform mat4 ProjTransform;
uniform vec4 lPosition_World;
uniform vec4 materialCoefficients;
 
smooth out vec4 vertOutFragColor;
smooth out vec2 vertOutTexCoords;
 
void main(void)
{
	// Get transformation matrices
	mat4 MV = ViewTransform * WorldTransform;
	mat4 MVP = ProjTransform * MV;

	// Get the position of the model
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

	// Get the normal of the model
	vec3 vector_N = normalize((transpose(inverse(MV)) * vec4(vertexNormal_modelspace,0)).xyz);
 
	// Find the eye and model position in View space
	vec4 vertexPos =  MV * vec4(vertexPosition_modelspace,1);
	vec3 vertexEyePos = normalize(-vertexPos.xyz);

    // Get the Light vector
    vec3 vector_L = normalize(-(ViewTransform * lPosition_World).xyz);

    float dotProd = max (0.f, dot(vector_N, vector_L));
	
	// We don't need any fragment colors at the moment
    //vertOutFragColor.rgb = color.rgb * dotProd;
    //vertOutFragColor.a = color.a;

    // Pass the texture coordinates
    vertOutTexCoords = vertexUV_modelspace;
}