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
	boudingCircle = new Physics(BOUNDING_CIRCLE, this);
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
		//printf("A collision will happen if you set this position\n");
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
	// rotate to the moving direction
	faceToMoveDirection(dir);

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
	//GLfloat velocity = moveSpeed;

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

Physics* Character::detectCollision()
{
	// detect whether the character collide with any other physic objects
	for (unsigned int i = 0; i < physic_objects.size(); i++)
	{
		if (boudingCircle->detectCollision(physic_objects[i]))
			return physic_objects[i];
	}
	return NULL;
}

void Character::botMove(GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	//GLfloat velocity = moveSpeed;
	glm::vec3 dir = faceDir * velocity;
	update(glm::translate(dir));

	// if a collision will happen, perform bounce, set a new random faceDir, else just move the bot
	position += dir;
	boudingCircle->updateCircle(glm::vec2(position.x, position.z), bounding_radius);
	Physics* bounceObj = detectCollision();
	if (bounceObj)
	{
		botBounce(bounceObj, 1);
	}
}

void Character::botBounce(Physics* obj, int flag)
{
	// if this bot is sleeping or the player, treat it as a static object
	if (status == SLEEP || lifeCycle == 0.0f)
		return;
	
	glm::vec3 normal = glm::vec3(0);

	// if bounced with a plane
	if (obj->getType() == BOUNDING_LINE)
		normal = obj->getNormal();
	else if (obj->getType() == BOUNDING_CIRCLE)
	{
		// if bounced with a circle, the normal is the vector between centers
		glm::vec2 normal2d = glm::normalize(boudingCircle->getCenter() - obj->getCenter());
		normal = glm::vec3(normal2d.x, 0, normal2d.y);
		printf("%d\n", obj->getCharacter());
		// also send the bounce information to the other obj, flag make sure there is no deadlock
		if(flag == 1)
			obj->getCharacter()->botBounce(this->boudingCircle, 2);
	}
	
	// get the reflection direction
	glm::vec3 bounceDir = getPlaneBounceDirection(normal, faceDir);

	//printf("bounceDir: ( %f %f %f )\n", bounceDir.x, bounceDir.y, bounceDir.z);

	// update the position
	position += bounceDir * BOUNCE_OFFSET;
	update(glm::translate(bounceDir * BOUNCE_OFFSET));
	boudingCircle->updateCircle(glm::vec2(position.x, position.z), bounding_radius);

	// rotate to the moving direction
	setFaceDir(bounceDir);
}

glm::vec3 Character::getPlaneBounceDirection(glm::vec3 normal, glm::vec3 moveDir)
{
	// get the bounce move direction with a given incident moveDir and plane's normal
	GLfloat incident_angle = acos(glm::dot(-glm::normalize(moveDir), glm::normalize(normal)));

	// check the direction, since acos only gives us value >= 0
	// rotate the normal with reflection angle, so if it rotates to the place of incident vector
	glm::vec3 normal_rotated = glm::rotate(incident_angle, worldUp) * glm::vec4(normal, 1.0f);
	float distance = (float)glm::length(glm::normalize(normal_rotated) - glm::normalize(-moveDir));
	GLfloat reflection_angle;
	if (distance > 0.01f)
	{
		// right angle for reflection
		reflection_angle = incident_angle;
	}
	else
	{
		// swap the angle sign
		reflection_angle = -incident_angle;
	}
	glm::vec3 bounceDir = glm::rotate(reflection_angle, worldUp) * glm::vec4(normal, 1.0f);
	bounceDir = glm::normalize(bounceDir);
	return bounceDir;
}

void Character::faceToMoveDirection(glm::vec3 dir)
{
	// let the character facing its moving direction 
	// If the character will move in a different direction than it's facing direction
	if (glm::length(glm::normalize(dir) - glm::normalize(faceDir)) > 0.01f)
	{
		// Rotate the character, translate it back to the origin and do the rotation
		update(glm::translate(-position));

		// Compute the angle of rotation
		GLfloat angle_radians = acos(glm::dot(glm::normalize(dir), glm::normalize(faceDir)));
		// Decide CW or CCW for rotation
		glm::vec3 face_rotated = glm::rotate(angle_radians, worldUp) * glm::vec4(faceDir, 1.0f);

		// If we should rotate CW
		float distance = ((float)glm::length(glm::normalize(face_rotated) - glm::normalize(dir)));
		if (distance > 0.001f)
		{
			angle_radians *= -1;
		}

		update(glm::rotate(angle_radians, worldUp));

		// Translate back to the position
		update(glm::translate(position));

		// Change the facing direction
		faceDir = glm::normalize(dir);
	}
}