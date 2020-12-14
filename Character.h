#pragma once
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "Node.h"
#include "Geometry.h"
#include "Physics.h"

#include <string>
#include <vector>

class Character : public Node
{
private:
	std::vector<Geometry*> frames;
	Geometry* currentFrame;

	Physics* boudingCircle;
	GLfloat bounding_radius = 0.3f;
	std::vector<Physics*> physic_objects;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	//GLfloat moveSpeed = 0.01f;
	GLfloat moveSpeed = 2.0f;
	glm::vec3 faceDir;

	// Bot control
	GLfloat lifeCycle = 0.0f;
	GLfloat stopGap = 0.0f;
	int status = 0;

	GLuint textureID = 0;
	GLuint shaderID = 0;
	Material* material = NULL;

public:
	Character(std::vector<std::string> frameFiles, int mode);
	~Character();

	void draw(glm::mat4 C);
	void update(glm::mat4 C);
	
	glm::vec3 getPosition() { return position; }
	GLboolean setPosition(glm::vec3 pos);
	void move(glm::vec3 dir);
	void keyControl(bool* keys, GLfloat deltaTime);

	void useTexture(GLuint id);
	void useShader(GLuint id);
	void useMaterial(Material* mat);

	Physics* getPhysics() { return boudingCircle; }
	Material* getMatrial() { return material; }

	void addCollisionPhysic(Physics* obj) { physic_objects.push_back(obj); }
	void deleteCollisionPhysic(Physics* obj);
	Physics* detectCollision();

	void botMove(GLfloat deltaTime);
	void botBounce(Physics* obj, int flag);
	glm::vec3 getPlaneBounceDirection(glm::vec3 normal, glm::vec3 moveDir);

	GLfloat getLifeCycle() { return lifeCycle; }
	void setLifeCycle(GLfloat l) { lifeCycle = l; }
	GLfloat getStopGap() { return stopGap; }
	void setStopGap(GLfloat s) { stopGap = s; }
	int getStatus() { return status; }
	void setStatus(const int s) { status = s; }
	void setFaceDir(glm::vec3 dir) { faceToMoveDirection(dir); }

	GLfloat getRandFloat(int min, int max) { return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))); }
	glm::vec2 getRandPoint(int x_min, int x_max, int y_min, int y_max) { return glm::vec2(getRandFloat(x_min, x_max), getRandFloat(y_min, y_max)); }

	void faceToMoveDirection(glm::vec3 dir);
};

