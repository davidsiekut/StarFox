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

uniform vec3 lAttenuation;
uniform vec3 lColor;
uniform vec4 materialCoefficients;

in vec3 v_color;
in vec3 normalVector;
in vec3 eyeVector;
in vec4 lightVector;

out vec3 color;

void main()
{
	vec3 N = normalize(normalVector);
	vec3 E = normalize(eyeVector);
	vec3 L = normalize(lightVector.xyz);

	float distance = length(lightVector);

	vec3 ambient = lColor * materialCoefficients.x;

	float attenuation;
	if (lightVector.w == 0.0)
	{
		attenuation = 1.0;
	}
	else
	{
		attenuation = 1.0 / (lAttenuation.x + lAttenuation.y * distance + lAttenuation.z * distance * distance);
	}

	vec3 diffuse = attenuation * lColor * materialCoefficients.y * max(dot(N, L), 0.0);
 	  
	vec3 specular = vec3(0);
	if (dot(N, L) >= 0.0) // light source on wrong side
	{
		specular = attenuation * lColor * materialCoefficients.z * pow(max(dot(reflect(-L, N), E), 0.0), materialCoefficients.w);
	}

	color = (ambient + diffuse + specular) * v_color;
}
