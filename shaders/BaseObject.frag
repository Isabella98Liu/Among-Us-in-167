#version 330 core

struct Light
{
	vec3 color;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec3 color;
};

in vec3 fragPos;
in vec3 fragNormal;

out vec4 fragColor;

uniform Material material;
uniform DirectionalLight directionalLight;

uniform vec3 eyePos;

// Reference: https://www.udemy.com/course/graphics-with-modern-opengl/
vec4 CalcLightByDirection(Light light, vec3 direction)
{
	vec3 ambientLight = material.ambient;
	
	float diffuseFactor = max(dot(normalize(fragNormal), normalize(direction)), 0.0f);
	vec3 diffuseLight = diffuseFactor * material.diffuse;
	
	vec3 fragToEye = normalize(eyePos - fragPos);
	vec3 reflectedLight = normalize(reflect(direction, normalize(fragNormal)));
	float specularFactor = max(dot(fragToEye, reflectedLight), 0.0f);
	specularFactor = pow(specularFactor, material.shininess * 128);
	vec3 specularLight = specularFactor * material.specular;
	
	return vec4(light.color * material.color * (ambientLight + diffuseLight + specularLight), 1.0f);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

void main()
{
	fragColor = CalcDirectionalLight();
}