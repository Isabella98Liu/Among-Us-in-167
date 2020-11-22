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

struct SpotLight
{
	PointLight base;
	vec3 direction;
	float angle;
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
//uniform PointLight pointLight;
//uniform SpotLight spotLight;

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

vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = fragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	vec4 color = CalcLightByDirection(pLight.base, direction);
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	return (color / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	//calculate the spot light factor, also decide whether or not to light current frag
	vec3 rayDirection = normalize(fragPos - sLight.base.position);
	float spotLightFactor = dot(rayDirection, sLight.direction);
	
	if(spotLightFactor > sLight.angle)
	{
		vec4 color = CalcPointLight(sLight.base);
		
		// scale the slFactor between [0, 1], otherwise the edge won't fade very well
		// (New.max - New.min) * (Origin.curr - Origin.Min) / (Origin.max - Origin.min)
		float scaler = (1.0f - 0.0f) * (spotLightFactor - sLight.angle) / (1.0f - sLight.angle);
		
		return color * spotLightFactor;
	} else {
		return vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	
}
void main()
{
	fragColor = CalcDirectionalLight();
}