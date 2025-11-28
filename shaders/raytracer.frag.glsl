#version 430 core

#define INV_GAMMA 0.4545454545

in vec2 vTexCoord;

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform sampler2D uTexture2D;

void main()
{
	vec3 color = texture(uTexture2D, vTexCoord).rgb;

	// Apply gamma correction for proper display
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}

