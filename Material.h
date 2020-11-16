#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material
{
private:
	glm::vec3 color;
	GLfloat shininess;
	glm::vec3 ambient, diffuse, specular;

public:

	Material(glm::vec3 ambientFactor, glm::vec3 diffuseFactor, glm::vec3 specularFactor, GLfloat shine, glm::vec3 colorFactor);
	~Material();

	void setMaterial(glm::vec3 ambientFactor, glm::vec3 diffuseFactor, glm::vec3 specularFactor, GLfloat shine, glm::vec3 colorFactor);
	void sendMatToShader(const int shaderID);

	glm::vec3 getAmbient() { return ambient; }
	glm::vec3 getDiffuse() { return diffuse; }
	glm::vec3 getSpecular() { return specular; }
	GLfloat getShiness() { return shininess; }
	glm::vec3 getColor() { return color; }

};

