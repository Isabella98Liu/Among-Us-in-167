#version 330 core

out vec4 fragColor;

in vec3 fragPos;

uniform vec3 eyePos;

void main()
{
	fragColor = vec4(1.0, 0.8, 0.0, 1.0);
}