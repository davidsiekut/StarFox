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
	mat4 MV = ViewTransform * WorldTransform;
	mat4 MVP = ProjTransform * MV;

	vec2 texCoord0 = vec2(materialCoefficients.x, materialCoefficients.z);

    //mat3 normalMatrix;
    //normalMatrix[0] = normalize (modelview[0].xyz);
    //normalMatrix[1] = normalize (modelview[1].xyz);
    //normalMatrix[2] = normalize (modelview[2].xyz);
 
    //vec3 N = normalize (normalMatrix * normal);
	vec3 vector_N = normalize((transpose(inverse(MV)) * vec4(vertexNormal_modelspace,0)).xyz);
 
    // Get vertex position in eye coordinates
    //vec4 vertexPos = modelview * vertex;
    //vec3 vertexEyePos = vertexPos.xyz / vertexPos.w;

	vec4 vertexPos = MV * vec4(vertexPosition_modelspace, 0.f);
	vec3 vertexEyePos = vertexPos.xyz / vertexPos.w;

    // Get vector to light source
    vec3 vector_L = normalize(-(ViewTransform * lPosition_World).xyz);

    float dotProd = max (0.f, dot(vector_N, vector_L));
 
    //vertOutFragColor.rgb = color.rgb * dotProd;
    //vertOutFragColor.a = color.a;
 
	vertOutFragColor.rgb = vertexColor.rgb * dotProd;
    //vertOutFragColor.a = vertexColor.a;

    // Pass along the texture coordinates
    vertOutTexCoords = texCoord0;

    // Don't forget to transform the geometry!
    gl_Position = MVP * vec4(vertexPosition_modelspace, 0.f);
}