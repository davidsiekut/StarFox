#version 330 core

smooth in vec2 vertOutTexCoords;
 
uniform sampler2D sampler;
//uniform sampler2D brightImage;
 
float bloomLevel;
float exposure;

out vec4 color;
 
void main(void)
{
	// Bloom intensity and exposure
	bloomLevel = 1.0f;
	exposure = 0.5f;

    // Texture and Blur
    vec4 baseImage = texture (sampler, vertOutTexCoords);
 
    // Blurring
    vec4 brightPass = textureLod (sampler, vertOutTexCoords, 0);
    vec4 blurColor1 = textureLod (sampler, vertOutTexCoords, 1);
    vec4 blurColor2 = textureLod (sampler, vertOutTexCoords, 2);
    vec4 blurColor3 = textureLod (sampler, vertOutTexCoords, 3);
    vec4 blurColor4 = textureLod (sampler, vertOutTexCoords, 4);
	vec4 blurColor5 = textureLod (sampler, vertOutTexCoords, 5);
	vec4 blurColor6 = textureLod (sampler, vertOutTexCoords, 6);
	vec4 blurColor7 = textureLod (sampler, vertOutTexCoords, 7);
 
    vec4 bloom = (brightPass + blurColor1 + blurColor2 + blurColor3 + blurColor4 + blurColor5 + blurColor6 + blurColor7);
 
    vec4 preColor = baseImage + bloomLevel * bloom;

    // Apply exposure
	color = (1.0 - exp2 (-preColor * exposure));
    color.a = 1.0;
}