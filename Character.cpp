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
