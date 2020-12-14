#pragma once
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "Character.h"

// Reference: https://learnopengl.com/In-Practice/2D-Game/Particles && Discussion Slides 

class ParticleSystem
{
private:

	struct Particle
	{
		glm::vec3 position, velocity;
		GLfloat lifeCycle = 0.0f;

		// Run one-time step
		void update(GLfloat deltaTime);
	};

	static const int MAX_PARTICLES = 200;
	GLfloat PARTICLE_LIFE = 1.5f;
	GLfloat PARTICLE_VELOCITY = 1.0f;

	Character* owner;
	GLfloat lifeCycle = PARTICLE_SYSTEM_LIFE;

	GLuint VAO, VBO;

	Particle particles[MAX_PARTICLES];
	glm::vec3 position_data[MAX_PARTICLES];

	unsigned int lastUsedParticle = 0;
	int findFirstUnusedParticle();
	void respawnParticle(Particle* particle);

public:
	ParticleSystem(Character* o);
	~ParticleSystem();

	// Update the position of particles if it's alive, offset is the direction from emiiter to the particle
	void update(GLfloat deltaTime);
	
	void draw(GLuint shader, glm::mat4 MVP);

	static GLfloat getRandFloat(int min, int max) { return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))); }
	
	GLfloat getLifeCycle() { return lifeCycle; }
};

