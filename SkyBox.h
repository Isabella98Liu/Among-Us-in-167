#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>
#include <iostream>

#include "stb_image.h"

using namespace std;

class SkyBox
{
private:
	vector<std::string> faces;

	GLuint textureID;

	GLuint VAO;
	GLuint VBO, EBO;

public:
	SkyBox(vector<std::string> faceFiles);
	~SkyBox();

	void loadMap();
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);

	GLuint getTexture() { return textureID; }
};

