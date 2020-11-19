#pragma once

#include "BaseObject.h"

#include <vector>

using namespace std;

class Sphere
{
private:
	GLuint VAO;
	GLuint VBO, EBO, NBO;

	//std::vector<glm::vec3> vertices;
	//std::vector<glm::vec3> normals;
	//std::vector<glm::ivec3> indices;
	int numsToDraw;

	glm::mat4 model;

public:
	Sphere(unsigned int stackCount, unsigned int sectorCount, unsigned int scale);
	~Sphere();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader, GLuint texture, glm::vec3 eyePos);
	void update();
};

