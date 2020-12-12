#include "Character.h"

Character::Character(std::vector<std::string> frameFiles, int mode)
{
	// Initialize geometry for each frame
	for (unsigned int i = 0; i < frameFiles.size(); i++)
	{
		Geometry* frame = new Geometry(frameFiles[i], mode);
		frames.push_back(frame);
	}

	// Set the first frame as default frame
	currentFrame = frames[0];

	// Set default position and directions
	position = glm::vec3(0);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	up = worldUp;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::normalize(glm::cross(front, up));
	faceDir = glm::vec3(0.0f, 0.0f, 1.0f);

	// Set the bouding circle for this character
	boudingCircle = new Physics(BOUNDING_CIRCLE);
	boudingCircle->updateCircle(position, bounding_radius);
}

Character::~Character()
{
	// Delete each frame
	for (unsigned int i = 0; i < frames.size(); i++)
	{
		delete frames[i];
	}

	delete boudingCircle;
}

void Character::draw(glm::mat4 C)
{
	currentFrame->draw(C);
}

void Character::update(glm::mat4 C)
{
	for (unsigned int i = 0; i < frames.size(); i++)
	{
		frames[i]->update(C);
	}
}

void Character::setPosition(glm::vec3 pos)
{
	update(glm::translate(pos - position));
	position = pos;
	boudingCircle->updateCircle(position, bounding_radius);
}

void Character::move(glm::vec3 dir)
{
	// If the character will move in a different direction than it's facing direction
	if (int(glm::length(glm::normalize(dir) - glm::normalize(faceDir))) > 0)
	{
		// Rotate the character, translate it back to the origin and do the rotation
		update(glm::translate(-position));
		
		// Compute the angle of rotation
		GLfloat angle_radians = acos(glm::dot(glm::normalize(dir), glm::normalize(faceDir)));
		// if the rotates pi/2, need to decide the direction
		if (angle_radians == glm::pi<float>() / 2)
		{
			if (faceDir.x == 0)
			{
				if (faceDir.z * dir.x > 0)
					angle_radians = glm::pi<float>() / 2;
				else
					angle_radians = -glm::pi<float>() / 2;
			}
			else if (faceDir.z == 0)
			{
				if (faceDir.x * dir.z < 0)
					angle_radians = glm::pi<float>() / 2;
				else
					angle_radians = -glm::pi<float>() / 2;
			}
		}

		update(glm::rotate(angle_radians, worldUp));

		// Translate back to the position
		update(glm::translate(position));

		// Change the facing direction
		faceDir = glm::normalize(dir);
	}


	// if the chatacter is colliding with other objects, don't perform move unless current move step will eliminate its collision status
	if (is_collision)
	{
		// check whether current step will release its collision status
		position += dir;
		boudingCircle->updateCircle(position, bounding_radius);

		// check all the objects its colliding with
		bool collide = false;
		for (unsigned int i = 0; i < collide_objects.size(); i++)
		{
			if (boudingCircle->checkCircleCollision(collide_objects[i])) {
				collide = true; 
				break;
			}
		}
	
		// if this move will release the character from all its colliding objects, we accept current move
		if (collide == false)
		{
			is_collision == false;
			// clean the collide list
			unsigned int collision_size = collide_objects.size();
			for (unsigned int i = 0; i < collision_size; i++)
				collide_objects.pop_back();
		}

		// reverse move
		position -= dir;
		boudingCircle->updateCircle(position, bounding_radius);
	}

	printf("Current colliding objects: %d\n", collide_objects.size());

	// if we reject current move because it will cause further collision
	if (is_collision)
		return;

	// Change the position and tranform
	update(glm::translate(dir));
	position += dir;
	boudingCircle->updateCircle(position, bounding_radius);

}

void Character::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W]) {
		move(front * velocity);
	}
	else if (keys[GLFW_KEY_S]) {
		move(-front * velocity);
	}
	else if (keys[GLFW_KEY_A]) {
		move(-right * velocity);
	}
	else if (keys[GLFW_KEY_D]) {
		move(right * velocity);
	}
}

void Character::useTexture(GLuint id)
{
	textureID = id;
	for (unsigned int i = 0; i < frames.size(); i++)
	{
		frames[i]->useTexture(id);
	}
}

void Character::useShader(GLuint id)
{
	shaderID = id;
	for (unsigned int i = 0; i < frames.size(); i++)
	{
		frames[i]->useShader(id);
	}
}

void Character::useMaterial(Material* mat)
{
	material = mat;
	for (unsigned int i = 0; i < frames.size(); i++)
	{
		frames[i]->useMaterial(mat);
	}
}

void Character::addCollisionPhysic(Physics* obj)
{
	// check if current collide objects list contains the element
	if (std::find(collide_objects.begin(), collide_objects.end(), obj) != collide_objects.end())
		return;
	else
		collide_objects.push_back(obj);
}