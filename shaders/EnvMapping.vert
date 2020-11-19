#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 fragNorm;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragNorm = mat3(transpose(inverse(model))) * normal;
	//fragPos = vec3(model * vec4(position, 1.0));
	//gl_Position = projection * view * vec4(fragPos, 1.0f);
	gl_Position = projection * view * model * vec4(position, 1.0);
	fragPos = (model * vec4(position, 1.0f)).xyz;
}
