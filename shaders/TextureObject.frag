#version 330 core


in vec3 fragNormal;
in vec3 fragPos;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex;

void main()
{
	//fragColor = texture(tex, texCoord) * CalcDirectionalLight();
	fragColor = texture(tex, texCoord);
}