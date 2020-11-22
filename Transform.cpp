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

	for (int i = 0; i < childs.size(); i++)	// loop through each child and call draw
	{
		childs[i]->draw(shaderProgram, M_new);
	}
}

void Transform::update(glm::mat4 C)
{
	MT = C * MT;
	//for (int i = 0; i < childs.size(); i++)	// loop through each child and call update
	//{
	//	childs[i]->update(C);
	//}
}

void Transform::addChild(Node* child)
{
	childs.push_back(child);
}

void Transform::addChilds(std::vector<Transform*> child)
{
	for (int i = 0; i < child.size(); i++)
	{
		childs.push_back(child[i]);
	}
}
