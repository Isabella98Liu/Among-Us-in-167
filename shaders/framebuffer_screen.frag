#version 330 core

in vec2 texCoord;

out fragColor;

uniform sampler2D screenTexture;

void main()
{
	vec3 color = texture(screenTexture, texCoord).rgb;
	fragColor = vec4(color, 1.0);
}