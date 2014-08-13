#version 330 core

smooth in vec2 vertOutTexCoords;
 
uniform sampler2D origImage;
uniform sampler2D brightImage;
 
float bloomLevel;
float exposure;

out vec4 color;
 
void main(void)
{
	// Bloom intensity and exposure
	bloomLevel = 1.0f;
	exposure = 0.5f;

    // Texture and Blur
    vec4 baseImage = texture (origImage, vertOutTexCoords);
 
    // Blurring
    vec4 brightPass = textureLod (brightImage, vertOutTexCoords, 0);
    vec4 blurColor1 = textureLod (brightImage, vertOutTexCoords, 1);
    vec4 blurColor2 = textureLod (brightImage, vertOutTexCoords, 2);
    vec4 blurColor3 = textureLod (brightImage, vertOutTexCoords, 3);
    vec4 blurColor4 = textureLod (brightImage, vertOutTexCoords, 4);
	vec4 blurColor5 = textureLod (brightImage, vertOutTexCoords, 5);
	vec4 blurColor6 = textureLod (brightImage, vertOutTexCoords, 6);
 
    vec4 bloom = (brightPass + blurColor1 + blurColor2 + blurColor3 + blurColor4 + blurColor5 + blurColor6);
 
    vec4 preColor = baseImage + bloomLevel * bloom;

    // Apply exposure
	color = (1.0 - exp2 (-preColor * exposure));
    color.a = 1.0;
}