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

	// Set move speed for the chatacter
	moveSpeed = 0.8f;

	// Set default position and directions
	position = glm::vec3(0);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	up = worldUp;

	front = glm::vec3(0.0f, 0.0f, -1.0f);
	right = glm::normalize(glm::cross(front, up));
	faceDir = glm::vec3(0.0f, 0.0f, 1.0f);
}

Character::~Character()
{
	// Delete each frame
	for (unsigned int i = 0; i < frames.size(); i++)
	{
		delete frames[i];
	}
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

void Character::rotate(GLfloat deg, glm::vec3 axis)
{
	for (unsigned int i = 0; i < frames.size(); i++)
	{
		frames[i]->rotate(deg, axis);
	}
}

void Character::setPosition(glm::vec3 pos)
{
	update(glm::translate(pos - position));
	position = pos;
}

void Character::move(glm::vec3 dir)
{
	// Check whether character is facing the direction it will move in
	if (int(glm::length(glm::normalize(dir) - glm::normalize(faceDir))) > 0)
	{
		// If move in different direction, rotate the character, translate it back to the origin and do the rotation
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
	}

	// Change the position and tranform
	update(glm::translate(dir));
	position += dir;

	// Change the facing direction
	faceDir = glm::normalize(dir);
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
