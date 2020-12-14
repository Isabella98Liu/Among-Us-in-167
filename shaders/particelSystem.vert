// Reference: https://learnopengl.com/In-Practice/2D-Game/Particles

#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 projection;		// model * projection * view
//uniform vec3 offset; 	// handle emitter's movement
//uniform vec3 color;

out vec4 particleColor;

void main()
{
	//float scale = 10.0f;
	vec3 color = vec3(0,0,0);
	gl_Position = projection * vec4(position, 1.0);
	particleColor = vec4(color, 1.0);
}