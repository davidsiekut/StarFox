#version 330 core

#define NUM_PASSES 7

smooth in vec2 vertOutTexCoords;
uniform sampler2D bloomSampler;

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

const float blurSize = 1.0f/512.0f;

out vec4 color;
 
void main(void)
{
	vec4 blurColor = vec4(0.f);
	for(int i = 0; i < NUM_PASSES; i++)
	{
		blurColor += texture2D(bloomSampler, vec2(vertOutTexCoords.x, vertOutTexCoords.y+gaussFilter[i].x*blurSize) )*gaussFilter[i].y;
	}

	color = blurColor; //* texture2D(originalSampler, vertOutTexCoords);
}