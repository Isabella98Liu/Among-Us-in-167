#ifndef _SPHERE_H_
#define _SPHERE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Node.h"

class Sphere : public Node
{
public:
	GLuint VAO = 0, VBO = 0, NBO = 0, EBO = 0;

	GLuint shaderID;
	GLuint textureID;

	int stackCount = 24;
	int sectorCount = 24;
	int numsToDraw;

	glm::mat4 model = glm::mat4(1);

	Sphere(int stackNumber, int sectorNumber, int scale);

	void draw(glm::mat4 C);
	void update(glm::mat4 C);
	void useShader(GLuint s) { shaderID = s; }
	void useTexture(GLuint t) { textureID = t; }
};

#endif