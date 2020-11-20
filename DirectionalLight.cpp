#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string objFilename, glm::vec3 lightColor, glm::vec3 lightDir)
{
	color = lightColor;
	direction = lightDir;
	lightModel = new Geometry(objFilename);

	// resize the model to fit in 1/4 inch of the screen
	glm::mat4 model = lightModel->getModel();
	model = glm::scale(glm::vec3(0.03f)) * model;
	lightModel->setModel(model);
}

DirectionalLight::~DirectionalLight()
{
	delete lightModel;
}

void DirectionalLight::sendLightToShader(const int shaderID)
{
	glUseProgram(shaderID);

	glUniform3fv(glGetUniformLocation(shaderID, "directionalLight.base.color"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shaderID, "directionalLight.direction"), 1, glm::value_ptr(direction));

	glUseProgram(0);
}

void DirectionalLight::translateLightModel(glm::vec3 translation)
{
	lightModel->translate(translation);
}