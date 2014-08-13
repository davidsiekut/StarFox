#version 330 core

uniform vec3 lAttenuation;
uniform vec3 lColor;
uniform vec4 lPosition_World; 
uniform vec4 materialCoefficients;

uniform sampler2D sampler;

in vec2 tex_coord;
in vec3 v_normal;

out vec4 color;

void main()
{
	vec4 tex_color = texture(sampler, tex_coord);
	color = tex_color;
}