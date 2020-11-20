#pragma once
#include "Light.h"
#include "Geometry.h"

class DirectionalLight : public Light
{
private:
	glm::vec3 direction;	// direction of the directional light
	Geometry* lightModel;		// the 3D visualized model for light

public:
	DirectionalLight(std::string objFilename, glm::vec3 lightColor, glm::vec3 lightDir);
	~DirectionalLight();

	Geometry* getModel() { return lightModel; };

	void sendLightToShader(const int shaderID);
	
	void translateLightModel(glm::vec3 translation);
};

