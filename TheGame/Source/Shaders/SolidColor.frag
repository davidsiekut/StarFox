#version 330 core

in vec3 normal;
in vec4 v_color;

out vec3 color;

void main()
{
	color = v_color.rgb;
}