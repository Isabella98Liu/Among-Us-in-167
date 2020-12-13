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
	GLboolean is_collision = false;
	std::vector<Physics*> collide_objects;
	std::vector<Physics*> physic_objects;

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;
	glm::vec3 worldUp;

	GLfloat moveSpeed = 1.0f;
	glm::vec3 faceDir;

	GLuint textureID = 0;
	GLuint shaderID = 0;
	Material* material = NULL;

public:
	Character(std::vector<std::string> frameFiles, int mode);
	~Character();

	void draw(glm::mat4 C);
	void update(glm::mat4 C);
	
	glm::vec3 getPosition() { return position; }
	void setPosition(glm::vec3 pos);
	void move(glm::vec3 dir);
	void keyControl(bool* keys, GLfloat deltaTime);

	void useTexture(GLuint id);
	void useShader(GLuint id);
	void useMaterial(Material* mat);

	Physics* getPhysics() { return boudingCircle; }

	void setCollision() { is_collision = true; }
	void releaseCollision() { is_collision = false; }
	void addCollisionPhysic(Physics* obj) { physic_objects.push_back(obj); }

	GLboolean detectCollision();
};

