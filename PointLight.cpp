#include "PointLight.h"

PointLight::PointLight(std::string objFilename, glm::vec3 lightColor, glm::vec3 lightPos, GLfloat exp, GLfloat lin, GLfloat con)
{
	color = lightColor;
	position = lightPos;
	lightModel = new Model(objFilename);

	exponent = exp;
	linear = lin;
	constant = con;

	// resize the model to fit in 1/4 inch of the screen
	glm::mat4 model = lightModel->getModel();
	model = glm::scale(glm::vec3(0.03f)) * model;
	lightModel->setModel(model);

	lightModel->translate(lightPos);
}

PointLight::~PointLight()
{
	delete lightModel;
}

void PointLight::sendLightToShader(const int shaderID)
{
	glUseProgram(shaderID);

	glUniform3fv(glGetUniformLocation(shaderID, "pointLight.base.color"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shaderID, "pointLight.position"), 1, glm::value_ptr(position));
	glUniform1f(glGetUniformLocation(shaderID, "pointLight.exponent"), exponent);
	glUniform1f(glGetUniformLocation(shaderID, "pointLight.linear"), linear);
	glUniform1f(glGetUniformLocation(shaderID, "pointLight.constant"), constant);

	glUseProgram(0);
}

void PointLight::translateLightModel(glm::vec3 translation)
{
	lightModel->translate(translation);
}