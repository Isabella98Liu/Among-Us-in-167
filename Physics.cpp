#include "Physics.h"

Physics::Physics(int type)
{
	bounding_volume_type = type;
}

Physics::~Physics()
{

}

GLboolean Physics::detectCollision(Physics* obj)
{
	if (obj->bounding_volume_type == BOUNDING_CIRCLE)
	{
		// compute the distance between two center
		GLfloat distance = glm::length(center - obj->getCenter());

		// if the distance is less than 2 * radius, set the collsion of the two objects
		if (distance < 2 * radius)
		{
			return true;
		}
		return false;
	}

	return false;
}