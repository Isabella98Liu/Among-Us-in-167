#pragma once

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Node
{
private:
	Node* parent = NULL;
	GLboolean disabled = false;

public:
	virtual void draw(glm::mat4 C) = 0;
	virtual void update(glm::mat4 C) = 0;

	void setParent(Node* p) { parent = p; }
	Node* getParent() { return parent; }

	GLboolean getDisable() { return disabled; }
	void disable() { disabled = true; }
	void enable() { disabled = false; }
};

