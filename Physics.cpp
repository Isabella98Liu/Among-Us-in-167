#include "Physics.h"

Physics::Physics(int type)
{
	bounding_volume_type = type;
}

Physics::~Physics()
{

}

GLboolean Physics::checkCircleCollision(Physics* circle2)
{
	// compute the distance between two center
	GLfloat distance = glm::length(center - circle2->getCenter());

	// if the distance is less than 2 * radius, set the collsion of the two objects
	if (distance < 2 * radius)
	{
		return true;
	}
	return false;
}