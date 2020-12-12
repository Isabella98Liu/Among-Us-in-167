#include "SpotLight.h"

SpotLight::SpotLight(std::string objFilename, int loadMode, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 lightDir, GLfloat exp, GLfloat lin, GLfloat con, GLfloat ang)
{
	color = lightColor;
	position = lightPos;
	direction = lightDir;

	lightModel = new Geometry(objFilename, loadMode);

	exponent = exp;
	linear = lin;
	constant = con;

	angle = ang;

	// resize the model to fit in 1/4 inch of the screen
	glm::mat4 model = lightModel->getModel();
	model = glm::scale(glm::vec3(0.02f)) * model;
	lightModel->setModel(model);

	lightModel->translate(lightPos);
}

SpotLight::~SpotLight()
{
	delete lightModel;
}

void SpotLight::sendLightToShader(const int shaderID)
{
	glUseProgram(shaderID);

	glUniform3fv(glGetUniformLocation(shaderID, "spotLight.base.base.color"), 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(shaderID, "spotLight.base.position"), 1, glm::value_ptr(position));
	glUniform1f(glGetUniformLocation(shaderID, "spotLight.base.exponent"), exponent);
	glUniform1f(glGetUniformLocation(shaderID, "spotLight.base.linear"), linear);
	glUniform1f(glGetUniformLocation(shaderID, "spotLight.base.constant"), constant);

	glUniform3fv(glGetUniformLocation(shaderID, "spotLight.direction"), 1, glm::value_ptr(direction));
	glUniform1f(glGetUniformLocation(shaderID, "spotLight.angle"), angle);

	glUseProgram(0);
}

void SpotLight::translateLightModel(glm::vec3 translation)
{
	lightModel->translate(translation);
}

void SpotLight::rotate(glm::vec3 start, glm::vec3 end)
{
	glm::vec3 axis = glm::cross(glm::normalize(start), glm::normalize(end));;
	glm::vec4 position_new = glm::rotate(0.02f, axis) * glm::vec4(position, 1.0f);
	position = glm::vec3(position_new.x, position_new.y, position_new.z);
	lightModel->rotate(0.02f, axis);
}
