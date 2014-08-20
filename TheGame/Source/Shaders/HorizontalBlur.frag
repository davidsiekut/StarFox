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
// Dror Ozgaon
//

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

const float blurSize = 1.0f/512.f;

out vec4 color;
 
void main(void)
{
	vec4 blurColor = vec4(0.f);
	for(int i = 0; i < NUM_PASSES; i++)
	{
		blurColor += texture2D(bloomSampler, vec2(vertOutTexCoords.x+gaussFilter[i].x*blurSize, vertOutTexCoords.y))*gaussFilter[i].y;
	}

	color = blurColor;// * texture2D(originalSampler, vertOutTexCoords);
}