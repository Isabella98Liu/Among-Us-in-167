#pragma once

#include <string>

#include "Node.h"
#include "irrKlang.h"

using namespace irrklang;

class SoundSystem : public Node
{
private:
	GLfloat lifeCycle = 0.0f;
	ISoundEngine* soundEngine = NULL;
	std::string soundFile;

public:
	SoundSystem(GLfloat lc, std::string fileName);
	~SoundSystem();

	void update(GLfloat deltaTime);
	void update(glm::mat4 C) { return; }

	void draw(glm::mat4 C) { return; }

	void play();
	void stop();
	
	GLfloat getLifeCycle() { return lifeCycle; }

};

