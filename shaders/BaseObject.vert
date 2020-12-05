#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex;

uniform mat4 PV;	// projection * view
uniform mat4 model;

out vec3 fragPos;
out vec3 fragNormal;

void main()
{
	gl_Position = PV * model * vec4(position, 1.0);
	
	fragPos = (model * vec4(position, 1.0)).xyz;
	fragNormal = mat3(transpose(inverse(model))) * normal;
}