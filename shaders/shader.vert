#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform bool normalShadding;

out vec3 fragNormal;
out vec3 fragPos;

void main()
{
    // OpenGL maintains the D matrix so you only need to multiply by P, V (aka C inverse), and M
    gl_Position = projection * view * model * vec4(position, 1.0);
	
	if(normalShadding)
	{
		fragNormal = normal;
	}else {
		fragNormal = mat3(transpose(inverse(model))) * normal;
	}
	fragPos = (model * vec4(position, 1.0f)).xyz;
}