#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#include "Object.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "CommonValues.h"

using namespace std;

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;

	GLuint VAO, VBO;
	GLfloat pointSize;

	PointCloud* nextObj;
	int morph_tag;

public:
	//PointCloud(GLfloat pointSize);
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();
	
	void draw(const glm::mat4& view, const glm::mat4& projection, GLuint shader);
	void update();

	void updatePointSize(GLfloat size);
	void spin(float deg);
	void morphing();

	void setNextObj(PointCloud* Obj){ nextObj = Obj; }
	float getRandomFloat() { return (float)(rand() % 101 - 50) / 50.0f; }
	std::vector<glm::vec3> getPoints() { return points; }
};

#endif
