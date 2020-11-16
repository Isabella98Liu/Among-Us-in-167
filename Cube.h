#ifndef _CUBE_H_
#define _CUBE_H_

#include "BaseObject.h"

#include <vector>

class Cube : public BaseObject
{
private:
	GLuint VAO;
	GLuint VBO, EBO;

public:
	Cube(float size);
	~Cube();

	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void spin(float deg);
};

#endif