#include "Physics.h"

Physics::Physics(int type, Character* c)
{
	bounding_volume_type = type;
	character = c;
}

Physics::~Physics()
{

}

GLboolean Physics::detectCollision(Physics* obj)
{
	// Detect the collision between two bouding sphere / circle
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

	// Detect the collision between the bouding sphere / circle and the bouding line
	if (obj->bounding_volume_type == BOUNDING_LINE)
	{
		glm::vec2 A = obj->getStartPoint();
		glm::vec2 B = obj->getEndPoint();
		glm::vec2 C = center;

		// if angle BAC > 90, just check the distance between AC
		GLfloat BAC = acos(glm::dot(glm::normalize(C - A), glm::normalize(B - A)));
		if (BAC > glm::pi<float>() / 2)
		{
			if (glm::length(C - A) <= radius) 
				return true;
			else
				return false;
		}
		else 
		{
			// else check the angle ABC with same algorithm
			GLfloat ABC = acos(glm::dot(glm::normalize(C - B), glm::normalize(A - B)));
			if (ABC > glm::pi<float>() / 2)
			{
				if (glm::length(C - B) <= radius)
					return true;
				else
					return false;
			}
			else
			{
				// else check the length of perpendicular vector from C to vecter AB
				GLfloat BAC = acos(glm::dot(glm::normalize(C - A), glm::normalize(B - A)));
				GLfloat distance = glm::length(C - A) * sin(BAC);
				if (distance <= radius)
					return true;
				else
					return false;
			}
		}
	}

	return false;
}