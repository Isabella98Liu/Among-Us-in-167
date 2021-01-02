#pragma once

#include "Light.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class DirectionalLight : public Light
{
private:
	glm::vec3 direction;	// direction of the directional light

public:
	DirectionalLight(glm::vec3 lightColor, glm::vec3 lightDir);
	~DirectionalLight();


	void sendLightToShader(const int shaderID);
	
};

