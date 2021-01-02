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

class Character;

class Physics
{
private:
	int bounding_volume_type = -1;	// type of the bounding volumn

	// properties for bounding circle / sphere in 2d
	glm::vec2 center = glm::vec2(0.0f);
	GLfloat radius = 0.0f;
	Character* character;

	// properties for bounding line in 2d
	glm::vec2 start = glm::vec2(0.0f);
	glm::vec2 end = glm::vec2(0.0f);
	glm::vec3 normal = glm::vec3(0.0f);

public:

	Physics(int type, Character* c);
	~Physics();
	
	void updateCircle(glm::vec2 c, GLfloat r) { center = c; radius = r; }
	void updateLine(glm::vec2 s, glm::vec2 e, glm::vec3 n) { start = s; end = e; normal = n; }

	GLboolean detectCollision(Physics* obj);

	Character* getCharacter() { return character; }
	glm::vec2 getCenter() { return center; }
	GLfloat getRadius() { return radius; }
	glm::vec2 getStartPoint() { return start; }
	glm::vec2 getEndPoint() { return end; }

	int getType() { return bounding_volume_type; }
	glm::vec3 getNormal() { return normal; }
};

