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
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 eyePos;
uniform Material material;
uniform DirectionalLight directionalLight;

// Calculate the light intensity to be used for discretized shading
float calcLightIntensity(vec3 direction)
{
	float diffuseFactor = max(dot(normalize(fragNormal), normalize(direction)), 0.0f);
	
	vec3 fragToEye = normalize(eyePos - fragPos);
	vec3 reflectedLight = normalize(reflect(direction, normalize(fragNormal)));
	float specularFactor = max(dot(fragToEye, reflectedLight), 0.0f);
	specularFactor = pow(specularFactor, material.shininess * 128);
	
	float intensity = diffuseFactor + specularFactor;
	return intensity;
}

void main()
{
	vec4 color = vec4(material.color, 1.0);
	//vec4 color = vec4(1.0, 0.5, 1.0, 1.0);
	
	// Discretized shading
	float lightIntensity = calcLightIntensity(directionalLight.direction);
	if(lightIntensity > 0.95)
		fragColor = vec4(1.0, 1.0, 1.0, 1.0) * color;
	else if(lightIntensity > 0.5)
		fragColor = vec4(0.7, 0.7, 0.7, 0.7) * color;
	else if(lightIntensity > 0.05)
		fragColor = vec4(0.35, 0.35, 0.35, 0.35) * color;
	else
		fragColor = vec4(0.1, 0.1, 0.1, 0.1) * color;
		
	// Detect sihouette edges
	vec3 fragToEye = normalize(eyePos - fragPos);
	float edge = max(dot(fragToEye, normalize(fragNormal)), 0.0f);
	if(edge < 0.6f)	
		fragColor = vec4(0.0, 0.0, 0.0, 1.0);	// draw black at edges

}