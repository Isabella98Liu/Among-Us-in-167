#version 330 core


in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	//fragColor = texture(tex, texCoord);
	
	fragColor = vec4(1.0, 1.0, 1.0, 1.0);
	
}