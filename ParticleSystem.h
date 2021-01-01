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

class ParticleSystem : public Node
{
private:

	struct Particle
	{
		glm::vec3 position, velocity;
		GLfloat lifeCycle = 0.0f;

		// Run one-time step
		void update(GLfloat deltaTime);
	};

	static const int MAX_PARTICLES = 700;
	GLfloat PARTICLE_LIFE = 0.8f;
	GLfloat PARTICLE_VELOCITY = 1.0f;
	GLfloat PARTICLE_SIZE = 2.5f;
	
	int type;		// define two types of particle system, one for character appear, another for disappear
	glm::vec3 position;	// emitter's position
	GLfloat lifeCycle = PARTICLE_SYSTEM_LIFE;

	GLuint VAO, VBO;
	GLuint shaderID = 0;
	glm::mat4 model = glm::mat4(1);

	Particle particles[MAX_PARTICLES];
	glm::vec3 position_data[MAX_PARTICLES];

	unsigned int lastUsedParticle = 0;
	int findFirstUnusedParticle();
	void respawnParticle(Particle* particle);

public:
	ParticleSystem(glm::vec3 pos, int t);
	~ParticleSystem();

	// Update the position of particles if it's alive, offset is the direction from emiiter to the particle
	void update(GLfloat deltaTime);
	void update(glm::mat4 C);
	
	void useShader(GLuint id) { shaderID = id; }
	void draw(glm::mat4 C);

	static GLfloat getRandFloat(int min, int max) { return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))); }
	glm::vec3 getBallPoint(float scale);

	GLfloat getLifeCycle() { return lifeCycle; }
};

