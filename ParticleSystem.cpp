#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Character* o)
{
	owner = o;

	// Generate buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO and store the point data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_PARTICLES, position_data, GL_STATIC_DRAW);
	// Enable Vertex Attribute 0 to pass point data through to the shader
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Unbind the VAO and VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::draw(GLuint shaderID, glm::mat4 MVP)
{
	// Actiavte the shader program 
	glUseProgram(shaderID);

	// Get the shader variable locations and send the uniform data to the shader 
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(MVP));

	// Bind the VAO
	glBindVertexArray(VAO);

	glPointSize(2.0f);

	// Draw the points
	glDrawArrays(GL_POINTS, 0, 3 * MAX_PARTICLES);

	// Unbind the VAO and shader program
	glBindVertexArray(0);
	glUseProgram(0);
}

void ParticleSystem::Particle::update(GLfloat deltaTime)
{
	if (lifeCycle > 0.0f)
	{
		position += deltaTime * velocity;
		lifeCycle -= deltaTime;
	}
}

void ParticleSystem::update(GLfloat deltaTime)
{
	// add new particles 5 each frame
	for (unsigned int i = 0; i < 5; i++)
	{
		int unusedParticle = findFirstUnusedParticle();
		respawnParticle(&particles[unusedParticle]);
	}

	// update existing particles
	for (unsigned int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].update(deltaTime);
		position_data[i] = particles[i].position;
	}

	// update the buffer data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * MAX_PARTICLES, position_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// update lifecycle
	lifeCycle -= deltaTime;
}

int ParticleSystem::findFirstUnusedParticle()
{
	for (unsigned int i = lastUsedParticle; i < MAX_PARTICLES; i++)
	{
		if (particles[i].lifeCycle < 0.01f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	for (unsigned int i = 0; i < MAX_PARTICLES; i++)
	{
		if (particles[i].lifeCycle < 0.01f)
		{
			lastUsedParticle = i;
			return i;
		}
	}
	// if didn't find, override the first one
	lastUsedParticle = 0;
	return 0;
}

void ParticleSystem::respawnParticle(Particle* particle)
{
	// generate a random direction
	glm::vec3 dir = glm::vec3(getRandFloat(-1, 1), getRandFloat(-1, 1), getRandFloat(-1, 1));
	particle->position = glm::vec3(owner->getPosition());
	particle->velocity = PARTICLE_VELOCITY * dir;
	particle->lifeCycle = PARTICLE_LIFE;
}