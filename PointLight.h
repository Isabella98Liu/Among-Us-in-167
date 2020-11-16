#pragma once
#include "Light.h"
#include "Model.h"

class PointLight : public Light
{
private:
	glm::vec3 position;		// position of the point light

	Model* lightModel;
	
	GLfloat exponent, linear, constant;

public:
	PointLight(std::string objFilename, glm::vec3 lightColor, glm::vec3 lightPos, GLfloat exp, GLfloat lin, GLfloat con);
	~PointLight();

	Model* getModel() { return lightModel; }

	void sendLightToShader(const int shaderID);

	void translateLightModel(glm::vec3 translation);

	void rotate(glm::vec3 start, glm::vec3 end);
	void scaling(double offset);

	glm::vec3 getLightCoef() { return glm::vec3(exponent, linear, constant); }
	void setLight(glm::vec3 coef, glm::vec3 colr) { color = colr; exponent = coef.x, linear = coef.y, constant = coef.z; }
};

