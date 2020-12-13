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
	boudingCircle->updateCircle(glm::vec2(position.x, position.z), bounding_radius);
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

GLboolean Character::setPosition(glm::vec3 pos)
{
	// detect collision before set new position
	glm::vec3 old_position = position;
	position = pos;
	boudingCircle->updateCircle(glm::vec2(position.x, position.z), bounding_radius);
	// if a collision will happen in this position, reverse and cease
	if (detectCollision())
	{
		printf("A collision will happen if you set this position\n");
		position = glm::vec3(0);
		boudingCircle->updateCircle(glm::vec2(position.x, position.z), bounding_radius);
		return false;
	}

	// if no collision will happen, accept the new position
	update(glm::translate(position - old_position));
	return true;
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

	// Check whether current step will release its collision status
	position += dir;
	boudingCircle->updateCircle(glm::vec2(position.x, position.z), bounding_radius);
	// if a collision will happen with current move, reverse move and cease
	if (detectCollision())
	{
		//printf("A collision will happen, cease the move!\n");
		position -= dir;
		boudingCircle->updateCircle(glm::vec2(position.x, position.z), bounding_radius);
		return;
	}

	//printf("( %f %f %f )\n", position.x, position.y, position.z);
	// if no collision is about to happen, accept the move
	update(glm::translate(dir));
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

void Character::deleteCollisionPhysic(Physics* obj)
{
	// delete the obj if it is in physic_objects
	int position = -1;
	for (unsigned int i = 0; i < physic_objects.size(); i++)
	{
		if (physic_objects[i] == obj)
			position = i;
	}
	if (position != -1)
		physic_objects.erase(physic_objects.begin() + position);
}

GLboolean Character::detectCollision()
{
	// detect whether the character collide with any other physic objects
	for (unsigned int i = 0; i < physic_objects.size(); i++)
	{
		if (boudingCircle->detectCollision(physic_objects[i]))
			return true;
	}
	return false;
}