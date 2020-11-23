#include "Transform.h"

Transform::Transform()
{

}

Transform::~Transform()
{

}

void Transform::draw(glm::mat4 C)
{
	glm::mat4 M_new = C * MT;	// get the new transform matrix include C

	for (int i = 0; i < childs.size(); i++)	// loop through each child and call draw
	{
		childs[i]->draw(M_new);
	}
}

void Transform::update(glm::mat4 C)
{
	MT = C * MT;
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
