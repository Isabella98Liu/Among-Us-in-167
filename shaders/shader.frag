#version 330 core

in vec3 fragNormal;
in vec3 fragPos;

out vec4 fragColor;

struct Light
{
	vec3 color;
};

struct DirectionalLight
{
	Light base;
	vec3 direction;
};

struct PointLight
{
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	vec3 color;
};

uniform Material material;
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform vec3 eyePosition;
uniform bool normalShadding;

// Reference: https://www.udemy.com/course/graphics-with-modern-opengl/
vec4 CalcLightByDirection(Light light, vec3 direction)
{
	vec3 ambientLight = material.ambient;
	
	float diffuseFactor = max(dot(normalize(fragNormal), normalize(direction)), 0.0f);
	vec3 diffuseLight = diffuseFactor * material.diffuse;
	
	vec3 fragToEye = normalize(eyePosition - fragPos);
	vec3 reflectedLight = normalize(reflect(direction, normalize(fragNormal)));
	float specularFactor = max(dot(fragToEye, reflectedLight), 0.0f);
	specularFactor = pow(specularFactor, material.shininess);
	vec3 specularLight = specularFactor * material.specular;
	
	return vec4(light.color * material.color * (ambientLight + diffuseLight + specularLight), 1.0f);
}

vec4 CalcDirectionalLight()
{
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight()
{
	vec3 direction = fragPos - pointLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	vec4 color = CalcLightByDirection(pointLight.base, direction);
	float attenuation = pointLight.exponent * distance * distance +
						pointLight.linear * distance +
						pointLight.constant;
	return (color / attenuation);
}

void main()
{
	if(normalShadding)
	{
		vec3 normColor = (normalize(fragNormal) + 1) / 2.0;
		fragColor = vec4(normColor, 1.0);
	} else {
		fragColor = CalcDirectionalLight() + CalcPointLight();
		//fragColor = CalcDirectionalLight();
		//fragColor = CalcPointLight();
	}
	
}