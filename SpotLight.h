#pragma once
#include "Light.h"
#include "Geometry.h"

class SpotLight : public Light
{
private:
	glm::vec3 position;		// position of the point light
	glm::vec3 direction;	// direction of the spot light

	Geometry* lightModel;

	GLfloat exponent, linear, constant;
	GLfloat angle;

public:
	SpotLight(std::string objFilename, int loadMode, glm::vec3 lightColor, glm::vec3 lightPos, glm::vec3 lightDir, GLfloat exp, GLfloat lin, GLfloat con, GLfloat ang);
	~SpotLight();

	Geometry* getModel() { return lightModel; }

	void sendLightToShader(const int shaderID);

	void translateLightModel(glm::vec3 translation);

	void rotate(glm::vec3 start, glm::vec3 end);

};

