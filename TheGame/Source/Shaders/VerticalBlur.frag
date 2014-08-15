#version 330 core

#define NUM_PASSES 7

smooth in vec2 vertOutTexCoords;
uniform sampler2D sampler;

const vec2 gaussFilter[NUM_PASSES] = vec2[]
(
	vec2(-3.0,	0.015625),
	vec2(-2.0,	0.09375),
	vec2(-1.0,	0.234375),
	vec2(0.0,	0.3125),
	vec2(1.0,	0.234375),
	vec2(2.0,	0.09375),
	vec2(3.0,	0.015625)
);

const float blurSize = 10.0f/512.0f;

out vec4 color;
 
void main(void)
{
	vec4 blurColor = vec4(0.f, 0.f, 0.f, 0.f);

	blurColor += texture2D(sampler, vec2(vertOutTexCoords.x, vertOutTexCoords.y+gaussFilter[0].x*blurSize) )*gaussFilter[0].y;
	blurColor += texture2D(sampler, vec2(vertOutTexCoords.x, vertOutTexCoords.y+gaussFilter[1].x*blurSize) )*gaussFilter[1].y;
	blurColor += texture2D(sampler, vec2(vertOutTexCoords.x, vertOutTexCoords.y+gaussFilter[2].x*blurSize) )*gaussFilter[2].y;
	blurColor += texture2D(sampler, vec2(vertOutTexCoords.x, vertOutTexCoords.y+gaussFilter[3].x*blurSize) )*gaussFilter[3].y;
	blurColor += texture2D(sampler, vec2(vertOutTexCoords.x, vertOutTexCoords.y+gaussFilter[4].x*blurSize) )*gaussFilter[4].y;
	blurColor += texture2D(sampler, vec2(vertOutTexCoords.x, vertOutTexCoords.y+gaussFilter[5].x*blurSize) )*gaussFilter[5].y;
	blurColor += texture2D(sampler, vec2(vertOutTexCoords.x, vertOutTexCoords.y+gaussFilter[6].x*blurSize) )*gaussFilter[6].y;

	blurColor.a = 0.8;
	color = blurColor;
}