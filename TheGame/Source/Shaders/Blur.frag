#version 330 core

smooth in vec2 vertOutTexCoords;
uniform sampler2D sampler;

const vec2 gaussFilter[7] = vec2[]
( 
	vec2(-3.0,	0.015625),
	vec2(-2.0,	0.09375),
	vec2(-1.0,	0.234375),
	vec2(0.0,	0.3125),
	vec2(1.0,	0.234375),
	vec2(2.0,	0.09375),
	vec2(3.0,	0.015625)
);

uniform vec2 scale;

out vec4 color;
 
void main(void)
{
	vec4 blurColor = vec4(0.f, 0.f, 0.f, 0.f);

	for( int i = 0; i < 7; i++ )
	{
		blurColor += texture2D( sampler, vec2(vertOutTexCoords.x+gaussFilter[i].x*scale.x, vertOutTexCoords.y+gaussFilter[i].x*scale.y ) )*gaussFilter[i].y;
	}
	blurColor.a = 0.15;
	color = blurColor * 3.0f;
}