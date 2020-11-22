#include "Material.h"

Material::Material(glm::vec3 ambientFactor, glm::vec3 diffuseFactor, glm::vec3 specularFactor, GLfloat shine, glm::vec3 colorFactor)
{
	ambient = ambientFactor;
	diffuse = diffuseFactor;
	specular = specularFactor;
	shininess = shine;
	color = colorFactor;
}

Material::~Material()
{

}

void Material::setMaterial(glm::vec3 ambientFactor, glm::vec3 diffuseFactor, glm::vec3 specularFactor, GLfloat shine, glm::vec3 colorFactor)
{
	ambient = ambientFactor;
	diffuse = diffuseFactor;
	specular = specularFactor;
	shininess = shine;
	color = colorFactor;
}

void Material::sendMatToShader(const int shader)
{
	glUseProgram(shader);

	// Get the shader material variable location and send the uniform data to the shader
	glUniform3fv(glGetUniformLocation(shader, "material.ambient"), 1, glm::value_ptr(ambient));
	glUniform3fv(glGetUniformLocation(shader, "material.diffuse"), 1, glm::value_ptr(diffuse));
	glUniform3fv(glGetUniformLocation(shader, "material.specular"), 1, glm::value_ptr(specular));
	glUniform1f(glGetUniformLocation(shader, "material.shininess"), shininess);
	glUniform3fv(glGetUniformLocation(shader, "material.color"), 1, glm::value_ptr(color));

	glUseProgram(0);
}