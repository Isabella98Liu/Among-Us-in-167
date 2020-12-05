#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 lightColor, glm::vec3 lightDir)
{
	color = lightColor;
	direction = lightDir;
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::sendLightToShader(const int shaderID)
{
	glUseProgram(shaderID);

	glUniform3fv(glGetUniformLocation(shaderID, "directionalLight.base.color"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shaderID, "directionalLight.direction"), 1, glm::value_ptr(direction));

	glUseProgram(0);
}
