#include "Transform.h"

Transform::Transform()
{

}

Transform::~Transform()
{

}

void Transform::draw(GLuint shaderProgram, glm::mat4 C)
{
	glm::mat4 M_new = C * MT;	// get the new transform matrix include C

	for (unsigned int i = 0; i < childs.size(); i++)	// loop through each child and call draw
	{
		childs[i]->draw(shaderProgram, M_new);
	}
}

void Transform::update(glm::mat4 C)
{
	MT = C;
}

void Transform::addChild(Node* child)
{
	childs.push_back(child);
}
