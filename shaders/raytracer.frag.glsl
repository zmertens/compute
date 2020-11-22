#version 430 core

#define INV_GAMMA 0.4545454545

in vec2 vTexCoord;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform sampler2D uTexture2D;

void main()
{
	FragColor = texture(uTexture2D, vTexCoord);
}


