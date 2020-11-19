#version 330 core

out vec4 fragColor;

in vec3 fragNorm;
in vec3 fragPos;

uniform vec3 eyePos;
uniform samplerCube skybox;

void main()
{
	vec3 I = normalize(fragPos - eyePos);
	vec3 R = reflect(I, normalize(fragNorm));
	fragColor = vec4(texture(skybox, R).rgb, 1.0);
}