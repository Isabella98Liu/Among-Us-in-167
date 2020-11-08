#include "PointLight.h"

Model* PointLight::lightModel;

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
	model = glm::scale(glm::vec3(0.02f)) * model;
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

void PointLight::rotate(glm::vec3 start, glm::vec3 end)
{
	glm::vec3 axis = glm::cross(start, end);
	glm::vec4 position_new = glm::rotate(0.02f, axis) * glm::vec4(position, 1.0f);
	position = glm::vec3(position_new.x, position_new.y, position_new.z);
	lightModel->rotate(start, end);
}

void PointLight::scaling(double offset)
{
	// get the direction from the pointlight to the center of the model, here is (0,0,0)
	glm::vec3 direction = position - glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 distance = direction * (float)offset * 0.02f;
	position += distance;
	lightModel->translate(distance);
}