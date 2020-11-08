#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Light
{
protected:
	glm::vec3 color;	// the color of the light

public:
	glm::vec3 getColor() { return color; }

	virtual void sendLightToShader(const int shaderID) = 0;
};

