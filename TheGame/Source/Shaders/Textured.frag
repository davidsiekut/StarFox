#version 330 core

uniform sampler2D gSampler;

in vec2 tex_coord;
in vec3 normal;
in vec4 v_color;

out vec3 color;

void main()
{
	vec4 vTexColor = texture2D(gSampler, tex_coord);
	color = vTexColor.xyz;
}