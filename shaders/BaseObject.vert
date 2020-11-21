#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 PV;	// projection * view
uniform mat4 model;

out vec3 fragPos;

void main()
{
	gl_Position = PV * model * vec4(position, 1.0);
	
	fragPos = (model * vec4(position, 1.0)).xyz;
}