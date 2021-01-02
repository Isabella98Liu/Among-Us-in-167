#include "SoundSystem.h"

SoundSystem::SoundSystem(GLfloat lc, std::string fileName)
{
	lifeCycle = lc;
	soundFile = fileName;
	soundEngine = createIrrKlangDevice();
	play();
}

SoundSystem::~SoundSystem()
{
	stop();
	delete soundEngine;
}

void SoundSystem::update(GLfloat deltaTime)
{
	lifeCycle -= deltaTime;
}


void SoundSystem::play()
{
	soundEngine->play2D(soundFile.c_str(), true);
}

void SoundSystem::stop()
{
	soundEngine->play2D(soundFile.c_str(), false);
}