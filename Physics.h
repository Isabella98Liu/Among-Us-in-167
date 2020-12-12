#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

class Physics
{
private:
	int bounding_volume_type = -1;	// type of the bounding volumn

	// properties for bounding circle / sphere in 2d
	glm::vec2 center = glm::vec2(0.0f);
	GLfloat radius = 0.0f;

	// properties for bounding line in 2d
	glm::vec2 start = glm::vec2(0.0f);
	glm::vec2 end = glm::vec2(0.0f);

public:
	Physics(int type);
	~Physics();
	
	void updateCircle(glm::vec2 c, GLfloat r) { center = c; radius = r; }
	void updateLine(glm::vec2 s, glm::vec2 e) { start = s; end = e; }

	GLboolean checkCircleCollision(Physics* circle2);

	glm::vec2 getCenter() { return center; }
	GLfloat getRadius() { return radius; }
};

